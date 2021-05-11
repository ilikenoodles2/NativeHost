#pragma once

#include "vendor/json.hpp"

#include <tuple>
#include <string>
#include <unordered_map>

template<typename T>
struct FunctionTraits
    : FunctionTraits<decltype(&T::operator())>
{
};

template<typename ClassType, typename ReturnT, typename... Args>
struct FunctionTraits<ReturnT(ClassType::*)(Args...) const>
    : FunctionTraits<ReturnT(*)(Args...)>
{
};

template<typename ClassType, typename ReturnT, typename... Args>
struct FunctionTraits<ReturnT(ClassType::*)(Args...)>
    : FunctionTraits<ReturnT(*)(Args...)>
{
};

template<typename ReturnT, typename... Args>
struct FunctionTraits<ReturnT(*)(Args...)>
{
    static constexpr int ArgCount = sizeof...(Args);
    using ArgTuple = std::tuple<Args...>;
    using ReturnType = ReturnT;
};

class Adapter
{
public:
    using Type = void(Adapter::*)(const nlohmann::json&, nlohmann::json&);

    Adapter() = default;

    template<typename T>
    Adapter(T& lambda)
    {
        auto lambdaPtr = &lambda;
        void* temp = &lambdaPtr;
        m_Obj = *static_cast<void**>(temp);

        auto method = &T::operator();
        temp = &method;
        m_Func = *static_cast<Type*>(temp);
    }

    void operator()(const nlohmann::json& in, nlohmann::json& out)
    {
        (this->*m_Func)(in, out);
    }
private:
    void* m_Obj;
    Type m_Func;
};

class FunctionBinds
{
public:
    template<auto func>
    static void Bind(const std::string& name)
    {
        using Traits = FunctionTraits<decltype(func)>;

        BindImpl<func>(name, std::make_index_sequence<Traits::ArgCount>{});
    }

    template<auto method, typename T>
    static void Bind(const std::string& name, T& obj)
    {
        using Traits = FunctionTraits<decltype(method)>;

        BindImpl<method>(name, obj, std::make_index_sequence<Traits::ArgCount>{});
    }

    static void Call(const nlohmann::json& in, nlohmann::json& out)
    {
        s_Binds[static_cast<std::string>(in[0])](in, out);
    }
private:
    static std::unordered_map<std::string, Adapter> s_Binds;

    template<auto func, size_t... Indices>
    static void BindImpl(const std::string& name, std::index_sequence<Indices...>)
    {
        using Traits = FunctionTraits<decltype(func)>;
        auto lambda = [](const nlohmann::json& in, nlohmann::json& out) 
        {
            if constexpr(std::is_same_v<Traits::ReturnType, void>)
                func(static_cast<std::tuple_element_t<Indices, Traits::ArgTuple>>(in[Indices + 1])...);
            else
                out = func(static_cast<std::tuple_element_t<Indices, Traits::ArgTuple>>(in[Indices + 1])...);
        };

        s_Binds.emplace(name, lambda);
    }

    template<auto method, typename T, size_t... Indices>
    static void BindImpl(const std::string& name, T& obj, std::index_sequence<Indices...>)
    {
        using Traits = FunctionTraits<decltype(method)>;

        T* objPtr = &obj;
        auto lambda = [objPtr](const nlohmann::json& in, nlohmann::json& out)
        {
            if constexpr (std::is_same_v<Traits::ReturnType, void>)
                (objPtr->*method)(static_cast<std::tuple_element_t<Indices, Traits::ArgTuple>>(in[Indices + 1])...);
            else
                out = (objPtr->*method)(static_cast<std::tuple_element_t<Indices, Traits::ArgTuple>>(in[Indices + 1])...);
        };

        s_Binds.emplace(name, lambda);
    }
};

std::unordered_map<std::string, Adapter> FunctionBinds::s_Binds;
#pragma once

class Logger
{
public:
	Logger() {};
	~Logger();

	void Init(void* window);
	void Update(const float timestep);

	void Resize(int width, int height);
};

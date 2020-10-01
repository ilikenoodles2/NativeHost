var s_NativePort = null;

function ConnectNative() {
    s_NativePort = chrome.runtime.connectNative("test");
    if (s_NativePort) {
        console.log("Connected to host");

        s_NativePort.onMessage.addListener(OnNativeMessage);
        s_NativePort.onDisconnect.addListener(OnNativeDisconnect);
    }
}

function CallFunction(args) {
    if (!s_NativePort) return;

    console.log("Sending: ", args);
    s_NativePort.postMessage(args);
}

function DisconnectNative() {
    if (!s_NativePort) return;

    s_NativePort.disconnect();
    s_NativePort = null;
    console.log("Host disconnected");
}

function OnNativeMessage(retVal) {
    console.log("Recieved: ", retVal);
}

function OnNativeDisconnect() {
    var errMsg = chrome.runtime.lastError.message;
    console.log(errMsg);
    s_NativePort = null;
}

ConnectNative();
CallFunction(["Add", 1, 2]);
CallFunction(["Sub", 2, 3]);
CallFunction(["Add", 3, 4]);
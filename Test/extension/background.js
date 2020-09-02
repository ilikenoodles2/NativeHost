// connects to the native messaging app
//
var s_NativePort = null;

function ConnectNative() {
    s_NativePort = chrome.runtime.connectNative("test");
    if (s_NativePort) {
        console.log("Connected to host");

        s_NativePort.onMessage.addListener(OnNativeMessage);
        s_NativePort.onDisconnect.addListener(OnNativeDisconnect);
    }
}

function SendNativeMessage(msg) {
    if (!s_NativePort) return;

    s_NativePort.postMessage(msg);
    console.log("Sending: " + JSON.stringify(msg));
}

function DisconnectNative() {
    if (!s_NativePort) return;

    s_NativePort.disconnect();
    s_NativePort = null;
    console.log("Host disconnected");
}

function OnNativeMessage(message) {
    console.log("Received: " + message);
}

function OnNativeDisconnect() {
    var errMsg = chrome.runtime.lastError.message;
    console.log(errMsg);
    s_NativePort = null;
}

ConnectNative();
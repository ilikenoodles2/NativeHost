var s_NativePort = null;
var s_ReceivedMessage = true; // Used to synchronize Native Host and Chrome

function ConnectNative() {
    s_NativePort = chrome.runtime.connectNative("test");
    if (s_NativePort) {
        console.log("Connected to host");

        s_NativePort.onMessage.addListener(OnNativeMessage);
        s_NativePort.onDisconnect.addListener(OnNativeDisconnect);
    }
}

function CallFunction(args) {
    if (!s_ReceivedMessage) {
        setTimeout(() => { CallFunction(args); }, 0);
    }
    else if (s_NativePort) {
        console.log("Sending: ", args);
        s_NativePort.postMessage(args);
        s_ReceivedMessage = false;
    }
}

function DisconnectNative() {
    if (!s_NativePort) return;

    s_NativePort.disconnect();
    s_NativePort = null;
    console.log("Host disconnected");
}

function OnNativeMessage(retVal) {
    s_ReceivedMessage = true;
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
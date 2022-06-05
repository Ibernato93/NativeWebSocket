# NativeWebSocket
Implementing an android app with a native websocket in C++.
<br>
The [Websocketpp](https://github.com/zaphoyd/websocketpp) library was used to implement the websocket.
<br>
In case you want to manage JSON messages, you can use the [nlohmann/json](https://github.com/nlohmann/json) library already included in this project.
<br>
In this project, currently only the implementation of a websocket without TLS protocol is available. If you want to implement the websocket with TLS protocol, in the jniLibs folder you will already find openssl built for android. The cmake.txt is also already configured to include openssl. So you just need to implement the websocket connection with the websocketpp library.

# License
Refer to the included libraries

# Contributions
Any help is appreciated to improve the project.

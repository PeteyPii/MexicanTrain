# Mexican Train

Under `engine/` is an engine to simulate games of the popular domino game *Mexican Train*.
Supports human play and AI play.
Requires Boost and Google Protocol Buffers to build (currently uses a simple makefile).

Under `website/` is the frontend to play games online with other people if the engine is running on a server.

Under `common/` are definitions common between the engine and the website, such as messages communicated between the two.

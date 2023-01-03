# Bloons TD 6 Server

A server, transparent proxy and network traffic viewing tool for BTD6.

## This project is currently barely a thing. Nothing is fleshed out and most of the features are missing.

## Development information

This code currently runs on Linux only. The main reason for this currently is the transparent proxy.
The proxy works by routing traffic from BTD6 to the proxy server. The server then accepts the connection and also creates a connection to the original destination. This is not only very platform specific, but on Linux it only takes one or two lines of code, but on Windows it takes a very large amount of code and probably several classes and a thread/process that is dedicated to just getting the original destinations.
If you want to implement this, by my guest, but I currently have no desire to as I don't use Windows. If you need a general idea of where to start, you can probably look at the repository for mitmproxy.

I want this to be as portable as possible, so if you're going to contribute keep that in mind please. Of course not everything can be perfect (See above)

This project uses Boost libraries, licenced under the Boost Software License 1.0

My code sucks, I know. Please feel free to fix my terrible code. Coding is pretty much just a hobby to me and I have a basic knowledge of C++

## Legal and Licensing

This program is licensed under the GNU GPL 2.0 license

This project, all code in this project, etc. are NOT related in any way to Ninja Kiwi or Bloons TD 6.
This is an entirely fan made project originally made to explore the game.

# gmod-interfaces-book
Tiny tool for making accessing Source Engine's interfaces simpler

This project was based on the idea of [iterating over interfaces](https://aixxe.net/2017/03/walking-interface-list) of Source Engine.

You no longer need to guess about which shared library contains target interface and care about its version.

# Usage

**It works fine only for both Window x86 and x86_64  and also Linux x86 !! SRCDS !!**

I haven't explored the client binaries yet, but they are coming soon (so do Linux x86_64 SRCDS support)

The only thing you need to do is to make object of `IFacesBook` and get access to target interface through `getInterface` method or overloaded `[]` operator:

```cpp
  IFacesBook  interfaces;
  
  ICvar* ICvarInterface = interfaces.getInterface<ICvar>();
  
  // NOTE: there are some interfaces that have two different versions on client and server side
  // so you should check how target interface is stored
  // EXAMPLE:
  
  INetworkStringTableContainer* stringTables_Client { interfaces.getInterface<INetworkStringTableContainer>() };
  if (interfaces.hasInterface("VEngineClientStringTable001"))
    stringTablesClient = static_cast<INetworkStringTableContainer*>(interfaces["VEngineClientStringTable001"]);
```

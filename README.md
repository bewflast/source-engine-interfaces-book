# source-engine-interfaces-book
Tiny tool for making accessing Source Engine's interfaces simpler

This project was based on the idea of [iterating over interfaces](https://aixxe.net/2017/03/walking-interface-list) of Source Engine.

You no longer need to guess about which shared library contains target interface and care about its version.

# Usage

**It works fine only for both Windows and Linux x86/x86-64 !! SRCDS !!**

I haven't explored the client binaries yet, but they are coming soon

The only thing you need to do is to use static method `getInterface` of `IFacesBook` class:

```cpp
  auto* ICvarInterface = static_cast<ICvar*>(IFacesBook::getInterface("ICvar"));
  
  // NOTE: there are some interfaces that have two different versions on client and server side at the same runtime
  // so you should check how target interface is stored
  // EXAMPLE:

  auto* stringTablesClient { static_cast<INetworkStringTableContainer*>(IFacesBook::getInterface("INetworkStringTableContainer")) };
  // if true then that means VEngineServerStringTable001 was found first
  // while traversing over interfaces list and now stored as INetworkStringTableContainer
  // but that interface also has second version that exists too at runtime
  if (IFacesBook::getInterface("VEngineClientStringTable001"))
	  stringTablesClient = static_cast<INetworkStringTableContainer*>(IFacesBook::getInterface("VEngineClientStringTable001"));
```

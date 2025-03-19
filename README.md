# source-engine-interfaces-book
Tiny tool for making accessing Source Engine's interfaces simpler

You no longer need to guess about which shared library contains target interface and care about its version.

# Usage

The only thing you need to do is to use function `sourceEngineInterfaces::book::getInterface`:

```cpp
  auto* ICvarInterface = static_cast<ICvar*>(sourceEngineInterfaces::book::getInterface("ICvar"));
  
  // NOTE: there are some interfaces that have two different versions on client and server side at the same runtime
  // EXAMPLE:

  auto* stringTablesClient { static_cast<INetworkStringTableContainer*>(sourceEngineInterfaces::book::getInterface("INetworkStringTableContainer","VEngineClientStringTable001")) };

  auto* stringTablesServer { static_cast<INetworkStringTableContainer*>(sourceEngineInterfaces::book::getInterface("INetworkStringTableContainer","VEngineServerStringTable001")) };
  
```

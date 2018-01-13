> **DEPRECATED** - This project was built as POW that you can change your skin using the packet handler. It was built for patch 7.24 and won't work anymore. There is a much more consistent way to change your skin if you really want this, see [this post](https://www.unknowncheats.me/forum/1966944-post7.html).


# QuickSkin
League of Legends Skin Changer (Faker) without touching game files.

## What does it do?
QuickSkin allows you to use all available skins without possessing them.
It does not modify any game (content) files.
Modifying the `League of Legends.exe` file *may* be necessary.

## Will other players see the skins too?
Nop, only you.

## So why should I want that?
At least for me, some skins give me a "smoother" feeling when playing the champ.
Maybe you experience the same.

## How does it work? Guide? Help?
It works although it's not a finished end-customer ready product.  
In case you still want to set it up:
1. Clone repository.
2. Compile using Visual Studio 2017 (all dependencies are included, no additional configuration should be necessary).
3. Create `quickskin.json` file in the directory where your `League of Legends.exe` is.
4. Use the following file format:
```
{ 
    "Jayce": 3,
    "Miss Fortune": 1,
    "Ahri": 5,
    "Darius": 3,
    
    // "Champion Name (case sensitive)": Skin Identifier,
    // "LeBlanc": 5,
    // and so on
}
```
5. Here comes the tricky part. You need to force the game client to load QuickSkin.
There are multiple ways to do that. You could either inject the file everytime you start a game (tedious) or you modify the [Import Address Table](https://en.wikipedia.org/wiki/Portable_Executable#Import_Table) (less tedious).
I used [StudPE](http://www.cgsoftlabs.ro/studpe.html) (freeware) to modify the IAT, which works.
I also thought about writing a `d3d9.dll` proxy which would get automatically loaded by the game, but thats currently just a thought.
6. When you found some way the load QuickSkin all you need to do is start a game and the correct skin should be used.

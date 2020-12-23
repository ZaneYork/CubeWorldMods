Mods for Cube World beta
===
Depends on [Cube-World-Mod-Launcher](https://github.com/ChrisMiuchiz/Cube-World-Mod-Launcher)

Localization Mod
---
A mod for localize the game's text resources, works with CSV config file.
CSV config file should be UTF-8 encoding.
* Column Definition

|Column         |Definition     |Optional|Remark                      |
| ------------- |:-------------:|:------:|:-------------------------- |
|A              |Origin Text    |No      |Game's origin text resource |
|B              |Replace Text   |No      |Text to replace with        |
|C              |Encoding       |No      |Text encoding               |
|D              |Begin Offset   |Yes     |Search begin offset         |
|E              |End Offset     |Yes     |Search end offset           |

* About Encoding

|Encoding Name  |Remark                                     |
| ------------- |:----------------------------------------- |
|Unicode        |UCS-BE encoding                            |
|ASCII          |Extende ASCII encoding                     |
|ASCII_HEX      |Same as ASCII,replace text encoded with hex|
  

Unlock Map Icon Mod
---

### Commands:

```
/map <ico|tag|all> <0|1>
```
### Examples:
```
/map ico 0
/map all 1
```

Infinite Stack Mod
---



Change Specialization Mod
---

### Commands:

```
/trans class <class_type>
/trans spec <0|1>
```

Class Types:
|Type ID |Name         |
| ------ |:----------- |
|1       |Warrior(战士)|
|2       |Ranger(游侠) |
|3       |Mage(法师)   |
|4       |Rogue(盗贼)  |

### Examples:
```
/trans class 2
/trans spec 1
```



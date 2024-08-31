## Unreal Importer 1.2.0 Changelog :

- New Features:
	- Compatibility with latest articy:draft X version
	- Support seen, unseen and fallback functionality
	- Feature property text localization
	- Return arbitrary localized properties in Blueprint
	- Improved compatibility with Unreal Engine 5.4
- Fixes:
	- Fix backup text display issue
	- Address Unreal warnings
	- Handle Unreal deprecations
	- Fix missing Blueprint category

## Unreal Importer 1.1.1 Changelog :

- Changes:
	- Split definitions and declarations for ExpressoType structs 
- Fixes:
	- Handle cast issues in Unreal Engine 5.4
	- Fix string field loading in import

## Unreal Importer 1.1.0 Changelog : 

- New Features
	- Support for Unreal Engine 5.4

## Unreal Importer 1.0.3 Changelog : 

- Changes:
	- Restructure string table generation to be macro compatible
	- Add ability to verify/update string table inclusion

## Unreal Importer 1.0.2 Changelog : 

- Fixes:
	- Minor compatibility issues
	- Avoid deprecated functionality

## Unreal Importer 1.0.1 Changelog : 

- New Features
	- Maintain data clones across world partitions
	- Allow escaping of rich text markup
	- Importing of voice over assets
	- Localisation of display names
- Changes:
	- Clean up removed classes from generated code
	- Abtract localisation out of interfaces
	- Log clones as normal events in runtime category
- Fixes:
	- Fixed crash on invalid rich text markup
	- Fixed hot reload crashing issue
	- Fixed incorrect localisation mappings in Slate UI
	- Fixed issues with some text extension variables

## Unreal Importer 1.0.0 Changelog :

- Support for articy:draft X

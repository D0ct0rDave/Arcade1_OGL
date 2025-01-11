// -----------------------------------------------------------------------------
#ifndef LevelH
#define LevelH
// -----------------------------------------------------------------------------
#ifndef GAMEAPIENTRY 
#define GAMEAPIENTRY __cdecl
#endif

typedef void (GAMEAPIENTRY *TLevelRenderFunc)();
typedef void (GAMEAPIENTRY *TLevelUpdateFunc)();
typedef void (GAMEAPIENTRY *TLevelInitPersistentFunc)();

extern TLevelRenderFunc				LevelRender;
extern TLevelUpdateFunc				LevelUpdate;
extern TLevelInitPersistentFunc		LevelInitPersistent;
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------

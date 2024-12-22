enable_language(RC)

add_custom_command(OUTPUT CameraPaths.res
        COMMAND ${CMAKE_RC_COMPILER}
        /l1036  # locale
        /c1252  # codepage
        /y  # Don't warn if there are duplicate control ID's
        /I ${CMAKE_CURRENT_LIST_DIR}/Resources
        /I ${CMAKE_CURRENT_LIST_DIR}/Sources/Compiler
        /fo ${CMAKE_CURRENT_BINARY_DIR}/CameraPaths.res
        ${CMAKE_CURRENT_LIST_DIR}/Resources/CameraPaths.rc
        DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/Resources/CameraPaths.rc
        )

add_library(CameraPaths SHARED
        Sources/Libraries/Libraries_CameraPaths.c
        Sources/Datas/Datas_CameraPaths.c
        Sources/Datas/Texts_CameraPaths.c
        CameraPaths.res
)
target_compile_definitions(CameraPaths PRIVATE CAMERAPATHDLL=)
target_include_directories(CameraPaths PRIVATE Includes)
target_link_libraries(CameraPaths
        ${CMAKE_CURRENT_BINARY_DIR}/CameraPaths.res
        Comctl32
        )
set_target_properties(CameraPaths
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${EDITOR_BINARIES_DIR}/Libs"
        LIBRARY_OUTPUT_DIRECTORY "${EDITOR_BINARIES_DIR}/Libs"
        RUNTIME_OUTPUT_DIRECTORY "${EDITOR_BINARIES_DIR}/Libs"
)

add_custom_command(OUTPUT Editor.res
        COMMAND ${CMAKE_RC_COMPILER}
        /nologo
        /l1036  # locale
        /c1252  # codepage
        /y  # Don't warn if there are duplicate control ID's
        /I ${CMAKE_CURRENT_LIST_DIR}/
        /fo ${CMAKE_CURRENT_BINARY_DIR}/Editor.res
        ${CMAKE_CURRENT_LIST_DIR}/Resources/Editor.rc
        DEPENDS
        ${CMAKE_CURRENT_LIST_DIR}/Resources/Editor.rc
        )

add_executable(Editor WIN32
        Sources/Compiler/Compiler_Compile.c
        Sources/Compiler/Compiler_Conversions.c
        Sources/Compiler/Compiler_Errors.c
        Sources/Compiler/Compiler_Generate.c
        Sources/Compiler/Compiler_Info.c
        Sources/Compiler/Compiler_Report.c
        Sources/Datas/Datas_ASCII.c
        Sources/Datas/Datas_Effects.c
        Sources/Datas/Datas_Game.c
        Sources/Datas/Datas_GlobalDefault.c
        Sources/Datas/Datas_GlobalVariables.c
        Sources/Datas/Datas_LastError.c
        Sources/Datas/Datas_Manager.c
        Sources/Datas/Datas_Map.c
        Sources/Datas/Datas_Menus.c
        Sources/Datas/Datas_Modes.c
        Sources/Datas/Datas_Things.c
        Sources/Datas/Datas_Thumbnails.c
        Sources/Datas/Datas_Triggers.c
        Sources/Datas/Texts_ASCII.c
        Sources/Datas/Texts_Bands.c
        Sources/Datas/Texts_Compiler.c
        Sources/Datas/Texts_Config.c
        Sources/Datas/Texts_Debug.c
        Sources/Datas/Texts_Disk.c
        Sources/Datas/Texts_Effects.c
        Sources/Datas/Texts_Files.c
        Sources/Datas/Texts_Global.c
        Sources/Datas/Texts_GlobalVariables.c
        Sources/Datas/Texts_Info.c
        Sources/Datas/Texts_LastError.c
        Sources/Datas/Texts_Libraries.c
        Sources/Datas/Texts_Manager.c
        Sources/Datas/Texts_Map.c
        Sources/Datas/Texts_Menus.c
        Sources/Datas/Texts_Report.c
        Sources/Datas/Texts_Requests.c
        Sources/Datas/Texts_Stats.c
        Sources/Datas/Texts_Things.c
        Sources/Datas/Texts_Thumbnails.c
        Sources/Datas/Texts_Triggers.c
        Sources/Datas/Texts_Undo.c
        Sources/Editor.c
        Sources/Files/Files_Areas.c
        Sources/Files/Files_Editor.c
        Sources/Files/Files_Functions.c
        Sources/Files/Files_Game.c
        Sources/Files/Files_Load.c
        Sources/Files/Files_Save.c
        Sources/Files/Files_Text.c
        Sources/Help/Help_Help.c
        Sources/Libraries/Libraries_Calls.c
        Sources/Main/Main_Debug.c
        Sources/Main/Main_Keyboard.c
        Sources/Main/Main_Startup.c
        Sources/Main/Main_Menu.c
        Sources/Map/Map_Areas.c
        Sources/Map/Map_Cameras.c
        Sources/Map/Map_Choice.c
        Sources/Map/Map_Display.c
        Sources/Map/Map_Find.c
        Sources/Map/Map_Functions.c
        Sources/Map/Map_Mouse.c
        Sources/Map/Map_Props.c
        Sources/Map/Map_Select.c
        Sources/Map/Map_Setup.c
        Sources/Map/Map_Sounds.c
        Sources/Map/Map_Stats.c
        Sources/Map/Map_Undo.c
        Sources/Misc/Misc_Display.c
        Sources/Misc/Misc_Edges.c
        Sources/Misc/Misc_Font.c
        Sources/Misc/Misc_Images.c
        Sources/Misc/Misc_Lists.c
        Sources/Misc/Misc_Maths.c
        Sources/Misc/Misc_Menus.c
        Sources/Misc/Misc_Requests.c
        Sources/Properties/Editor/Properties_Editor_Config.c
        Sources/Properties/Editor/Properties_Editor_Registry.c
        Sources/Properties/GlobalVariables/Properties_Global_Creatures.c
        Sources/Properties/GlobalVariables/Properties_Global_Default.c
        Sources/Properties/GlobalVariables/Properties_Global_Elements.c
        Sources/Properties/GlobalVariables/Properties_Global_Functions.c
        Sources/Properties/Map/Properties_Map_Alliances.c
        Sources/Properties/Map/Properties_Map_Computer.c
        Sources/Properties/Map/Properties_Map_Creatures.c
        Sources/Properties/Map/Properties_Map_Global.c
        Sources/Properties/Map/Properties_Map_Options.c
        Sources/Properties/Map/Properties_Map_Players.c
        Sources/Properties/Map/Properties_Map_Texts.c
        Sources/Properties/Map/Properties_Map_Variables.c
        Sources/Properties/Map/Properties_Map_Voices.c
        Sources/Things/Things_Display.c
        Sources/Things/Things_Functions.c
        Sources/Things/Things_Heros.c
        Sources/Things/Things_Manager.c
        Sources/Things/Things_Paths.c
        Sources/Things/Things_RectEx.c
        Sources/Things/Things_Selection.c
        Sources/Thumbnail/Thumbnail_Functions.c
        Sources/Triggers/Triggers_ActionPoints.c
        Sources/Triggers/Triggers_Actions.c
        Sources/Triggers/Triggers_Display.c
        Sources/Triggers/Triggers_Functions.c
        Sources/Triggers/Triggers_Select.c
        Sources/Triggers/Triggers_Texts.c
        Sources/Triggers/Triggers_Triggers.c
        Sources/Triggers/Triggers_Variables.c
        Sources/Windows/Windows_Compiler.c
        Sources/Windows/Windows_Disk.c
        Sources/Windows/Windows_Fullmap.c
        Sources/Windows/Windows_Functions.c
        Sources/Windows/Windows_Info.c
        Sources/Windows/Windows_MainWindowProc.c
        Sources/Windows/Windows_Minimap.c
        Sources/Windows/Windows_Safe.c
        Sources/Windows/Windows_Splash.c
        Sources/Windows/Windows_StatusWindow.c
        Sources/Windows/Windows_Thumbnail.c
        Sources/Windows/Windows_Toolbar.c
        Sources/Windows/Windows_ZoomButton.c
        Editor.res
)
target_include_directories(Editor PRIVATE Includes)
target_link_libraries(Editor
        ${CMAKE_CURRENT_BINARY_DIR}/Editor.res
        Comctl32 Shlwapi Winmm
)
add_dependencies(Editor CameraPaths)
set_property(TARGET Editor PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set_target_properties(Editor
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${EDITOR_BINARIES_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${EDITOR_BINARIES_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${EDITOR_BINARIES_DIR}"
)

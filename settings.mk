#
# Settings
#
 
MXE=no
MXE_DIR=/mnt/e/Libraries/MXE/mxe
SLIENT=yes

COLORS := yes
 
CXXFLAGS := -static -std=c++17 -O2 -Ic:/Biblioteki/imgui-1.60/include
 
#$(error Sprawdz plik konfiguracyjny settings.mk ;D)
# Zmień na odpowiednie ścieżki (btw, ważne dla Windowsa chyba)
 
 
INC_DIR_SFML_32 := c:/Biblioteki/GCC-2.4.2-SFML/include
BIN_DIR_SFML_32 := c:/Biblioteki/GCC-2.4.2-SFML/lib
 
 
INC_DIR_SFML_64 := c:/Biblioteki/sfml_x64_seh/include
LIB_DIR_SFML_64 := c:/Biblioteki/sfml_x64_seh/lib
BIN_DIR_SFML_64 := c:/Biblioteki/sfml_x64_seh/bin
 
INC_DIR_BOOST_32 := c:/Biblioteki/boost_1_65_1/include
BIN_DIR_BOOST_32 := 
INC_DIR_BOOST_64 := c:/Biblioteki/boost_1_65_1/include
BIN_DIR_BOOST_64 :=
 
LDFLAGS_STATIC_SFML := -Lc:/Biblioteki/imgui-1.60/lib -limgui_MinGW-7.2.0-64 -lsfml-audio-s -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -ljpeg -lopengl32 -lwinmm -lgdi32 -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lws2_32 -lwinmm -DSFML_STATIC
 
 
HEADER_INC_SOL2 := c:/Biblioteki/lua/sol
 
INC_DIR_LUA_64 := c:/Biblioteki/lua/lua-5.3.4/include
LIB_DIR_LUA_64 := c:/Biblioteki/lua/lua-5.3.4/lib
BIN_DIR_LUA_64 := c:/Biblioteki/lua/lua-5.3.4/bin
 
HEADER_INC_JSON := c:/Biblioteki/json
CXXFLAGS_STATIC_BOOST  :=
LDFLAGS_STATIC_BOOST  :=
CXXFLAGS_DYNAMIC_BOOST :=
LDFLAGS_DYNAMIC_BOOST :=
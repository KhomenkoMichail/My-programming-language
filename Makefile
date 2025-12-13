# Флаги компиляции .cpp
flags = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

# Общие файлы
common_all = COMMON/include/helpingFunctions.h COMMON/include/nameTableConsts.h COMMON/include/nameTableStack.h COMMON/include/operatorsArray.h COMMON/include/structAccessFunctions.h COMMON/include/structsAndConsts.h COMMON/include/treeFunctions.h

# Общие cpp. файлы
common_cpp = COMMON/src/helpingFunctions.cpp COMMON/src/nameTableStack.cpp COMMON/src/structAccessFunctions.cpp COMMON/src/treeFunctions.cpp

# Файлы .cpp для сборки frontend
front_cpp = frontend/src/main.cpp frontend/src/lexicalAnalysis.cpp frontend/src/syntacticAnalysis.cpp frontend/src/saveTreeInFile.cpp

# Файлы frontend
front_all = frontend/include/DSL.h frontend/include/lexicalAnalysis.h frontend/include/syntacticAnalysis.h frontend/include/saveTreeInFile.h

# Файлы .cpp для сборки backend
back_cpp = backend/src/backendMain.cpp backend/src/backendTreeCtor.cpp backend/src/asmProgramWriter.cpp

# Файлы backend
back_all = backend/include/backendTreeCtor.h backend/include/asmProgramWriter.h

# Компиляция frontend
build-f: $(front_all) $(common_cpp) $(common_all)
	@echo -------------------------------------------------------------------------
	g++ $(front_cpp) $(common_cpp) $(flags) -o frontend/build/frontend.exe

# Компиляция backend
build-b: $(back_all) $(common_cpp) $(common_all)
	@echo -------------------------------------------------------------------------
	g++ $(back_cpp) $(common_cpp) $(flags) -o backend/build/backend.exe

run-f:
	./frontend/build/frontend.exe

run-b:
	./backend/build/frontend.exe

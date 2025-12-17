# Флаги компиляции .cpp
flags = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

# Общие файлы
common_all = COMMON/include/helpingFunctions.h COMMON/include/nameTableConsts.h COMMON/include/nameTableStack.h COMMON/include/operatorsArray.h COMMON/include/structAccessFunctions.h COMMON/include/structsAndConsts.h COMMON/include/treeFunctions.h COMMON/include/saveTreeInFile.h COMMON/include/readTreeFromFileFunc.h COMMON/include/numbersArray.h

# Общие cpp. файлы
common_cpp = COMMON/src/helpingFunctions.cpp COMMON/src/nameTableStack.cpp COMMON/src/structAccessFunctions.cpp COMMON/src/treeFunctions.cpp COMMON/src/saveTreeInFile.cpp COMMON/src/readTreeFromFileFunc.cpp

# Файлы .cpp для сборки frontend
front_cpp = frontend/src/main.cpp frontend/src/lexicalAnalysis.cpp frontend/src/syntacticAnalysis.cpp

# Файлы frontend
front_all = frontend/include/DSL.h frontend/include/lexicalAnalysis.h frontend/include/syntacticAnalysis.h

# Файлы .cpp для сборки backend
back_cpp = backend/src/backendMain.cpp backend/src/asmProgramWriter.cpp backend/src/sourceFileParser.cpp

# Файлы backend
back_all = backend/include/asmProgramWriter.h backend/include/backendConsts.h backend/include/sourceFileParser.h

# Файлы .cpp для сборки middleend
middle_cpp = middleend/src/middleEndMain.cpp middleend/src/middleendOptimization.cpp

# Файлы middleend
middle_all = middleend/include/middleendOptimization.h

# Файлы .cpp для сборки reverse-frontend
reverse_cpp = reverse-frontend/src/reverseFrontendFunctions.cpp reverse-frontend/src/reverseFrontendMain.cpp

# Файлы reverse-frontend
reverse_all = reverse-frontend/include/reverseFrontendFunctions.h reverse-frontend/include/reverseFrontendDSL.h

# Компиляция frontend
build-f: $(front_all) $(common_cpp) $(common_all)
	@echo -------------------------------------------------------------------------
	g++ $(front_cpp) $(common_cpp) $(flags) -o frontend/build/frontend.exe

# Компиляция backend
build-b: $(back_all) $(common_cpp) $(common_all)
	@echo -------------------------------------------------------------------------
	g++ $(back_cpp) $(common_cpp) $(flags) -o backend/build/backend.exe

# Компиляция middleend
build-m: $(middle_all) $(middle_cpp) $(common_all)
	@echo -------------------------------------------------------------------------
	g++ $(middle_cpp) $(common_cpp) $(flags) -o middleend/build/middleend.exe

# Компиляция reverse-frontend
build-r: $(reverse_all) $(reverse_cpp) $(common_all)
	@echo -------------------------------------------------------------------------
	g++ $(reverse_cpp) $(common_cpp) $(flags) -o reverse-frontend/build/reverse.exe


run-f:
	./frontend/build/frontend.exe

run-b:
	./backend/build/frontend.exe

run-m:
	./middleend/build/middleend.exe
run-r:
	./reverse-frontend/build/reverse.exe

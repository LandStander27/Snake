


run:
	IF NOT EXIST "%CD%\bin" mkdir "bin"
	windres icon.rc -O coff -o icon.res
	copy "*.dll" "bin\."
	IF NOT EXIST "%CD%\bin\assets" mkdir "bin\assets"
	copy "assets\*.*" "bin\assets\."
	g++ -I src\include -L src\lib -o bin\snake.exe snake.cpp icon.res -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -static-libgcc -static-libstdc++ -mwindows


debug:
	IF NOT EXIST "%CD%\bin" mkdir "bin"
	windres icon.rc -O coff -o icon.res
	copy "*.dll" "bin\."
	IF NOT EXIST "%CD%\bin\assets" mkdir "bin\assets"
	copy "assets\*.*" "bin\assets\."
	g++ -I src\include -L src\lib -o bin\snake.exe snake.cpp icon.res -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -static-libgcc -static-libstdc++

clean:
	del icon.res
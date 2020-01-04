b: MainWin.cpp
	g++ MainWin.cpp include/Application.cpp include/BarContainer.cpp include/BubbleSort.cpp include/Camera.cpp include/CameraController.cpp include/Graphics.cpp include/Funclib.cpp include/HeapSort.cpp include/InputUtility.cpp include/InsertionSort.cpp include/Matrix.cpp include/MergeSort.cpp include/QuickSort.cpp include/SelectionSort.cpp include/Sort.cpp include/SortingController.cpp include/ShellSort.cpp include/Voronoi.cpp include/UI.cpp -o main.out -L/usr/lib/x86_64-linux-gnu -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -Wall -g -pthread

r: MainWin.cpp
	./main.out

br: MainWin.cpp
	make b && make r
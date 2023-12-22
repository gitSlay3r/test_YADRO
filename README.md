Переключение между тестами и приложением осуществляется с помощью директив препроцессора RUN_TESTS и RUN_MAIN 
Компиляция проекта с помощью команды:
clang++ -Iinclude -DRUN_MAIN src/main.cpp src/FileTapeDevice.cpp src/TapeSorter.cpp src/ConfigManager.cpp -o TapeSorterApp.exe
Использование:
.\TapeSorterApp.exe test_files/large_input_data.dat test_files/large_output_data.dat 
На вход подается бинарный файл, на выходе также бинарный файл
main.cpp:
1)block_processing - колличество обрабатываемых данных(на одной временной ленте)
2)все временные ленты хранятся в директории /tmp и удаляются после работы программы 
3)использован метод сортировки merge sort
файл config.txt находится в корне проекта, в случае если он отсутсвует применяются задержки по умолчанию 
Для тестирования применялись google test 

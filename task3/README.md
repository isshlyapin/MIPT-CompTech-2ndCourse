# Сравнение 3 механизмов межпроцессорного взаимодействия

## Параметры, установленные в системе, где производились все замеры

1) shmmax = 18446744073692774399 
2) msgmax = 8192
3) pipe-max-size = 1048576

![](https://github.com/isshlyapin/MIPT-CompTech-2ndCourse/blob/main/task3/image/grafic1.svg)

![](https://github.com/isshlyapin/MIPT-CompTech-2ndCourse/blob/main/task3/image/grafic2.svg)

## Запуск на своей машине

1) Клонируйте репозиторий
```
git clone https://github.com/isshlyapin/MIPT-CompTech-2ndCourse.git
```

2) Перейдите в директорию проекта
```
cd MIPT-CompTech-2ndCourse/task3
```

3) Для тестирования передачи данных между процессами создадим файл размером 512мб
```
time dd if=/dev/urandom of=image/input.txt bs=1048576 count=512
```

4) Для сборки проекта нам необходима утилита CMake и библиотека openssl
```
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
```
```
cmake --build build
```

5) Для запуска 
```
./bin/interprocess_communication
```
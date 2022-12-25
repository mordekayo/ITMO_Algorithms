# ITMO_Algorithms

Добрый день уважаемый Эдуард Александрович.

1 лабораторная:  Ничего интересного
2 лабораторная: Сдана
3 лабораторная: Сдана
4 лабораторная:

Важные детали реализации. 

  1. FSA:
  Новые блоки инициализируются только когда есть что инициализировать и фрилист пуст(то есть есть зачем инициализировать). В дебажной версии слева и справа от каждого блока памяти располагается дебажный блок, размером с блок памяти. Индекс следующего элемента располагается прямо в блоке памяти. Во время освобождения памяти мы проверяем относится ли блок к реальной памяти какой либо страницы и валидны ли его дебажные флаги.
  2. Coalesce:
  Структура блока выглядит так:

  * Дебажный флаг 
  * Структура содержащая указатели на соседей, размер блока памяти отдаваемой пользовтелю и флаг занятости
  * Память отдаваемая пользователю
  * Дебажный флаг
  
       При этом внутри памяти отдаваемой пользователю хранятся указатели на следующий и предыдущий элементы списка свободных блоков

Вся разработка велась в ветке main. Ознакомится с 4ой лабораторной работой в формате пул реквеста вы можете в единственном из доступных пул реквестов. Это пул реквест ветки отколотой от мейна после окончания разработки 4ой лабораторной работы, в ветку отколотую от мейна до начала лабораторной работы

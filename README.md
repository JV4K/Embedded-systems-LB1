# ЛБ 1 Встраиваемые системы

---
Тема лабораторной работы:

## Знакомство с SPI. Применение библиотек при разработке ПО под STM32

### Цель работы

Обеспечить прием и расшифровку сообщения, набираемого пользователем с помощью кнопки на отладочной плате с кода Морзе на русский язык. Полученный текст вывести на светодиодный модуль P10.

### Прием и расшифровка сообщения

Код Морзе - шифр, позволяющий представлять текстовые сообщения в виде последовательности точек, тире и пробелов. Для каждой буквы используемого алфавита определяется уникальный набор точек и тире, который можно однозначно расшифровать.
На практике передача сообщений, зашифрованных кодом Морзе осуществляется при помощи разного рода импульсов. Вначале принимается короткий базовый временной интервал (далее просто интервал). Точка соответствует импульсу, длительностью один интервал, тире - три интервала. Пауза между элементами одного знака - один интервал, между знаками в слове - 3 интервала, между словами в сообщении - 7 интервалов.
Для организации приема сообщения была использована отладочная плата ST Nucleo-144 на базе МК семейства STM32F429. Данная плата имеет пользовательскую кнопку, которую можно использовать для набора сообщений.
Для определения момента нажатия кнопки, пин C13 микроконтроллера, к которому он подключена, был сконфигурирован для генерации прерываний по фронту и спаду с помощью STM32 CubeMX. В контроллере прерываний NVIC был включён обработчик прерываний для линий 15:10, среди которых вектор прерывания выбранного пина.

<p align="center" width="100%">
    <img width="85%" src="./Images/Pasted%20image%2020240102154734.png" alt="CubeMX EXTI setup">
</p>

Для преобраования входного сигнала с кнопки в последовательность точек и тире необходимо определять логические уровни на ножке в моменты срабатывания прерывания, а также считать время между прерываниями по нажатию и отпусканию кнопки. Алгоритм, используемый в прерывании по изменению уровня:

<p align="center" width="100%">
    <img width="80%" src="./Images/Pasted%20image%2020240102224646.png">
</p>

Когда производится обработка точек и тире, необходимо произвести обработку различного рода пауз - между элементами символа, символами в слове и словами в сообщении. Для этого необходимо с помощью таймера вести постоянный счет времени после срабатывания прерывания. В проекте с этой целью таймер TIM2 сконфигурирован на генерацию прерываний с частотой 500 Гц. В прерывании происходит проверка текущего значения флага, отображающего состояние кнопки, и, в зависимости от этого происходит вычисление времени, в течение которого кнопка находится в состояниях "нажата" или "отпущена".

```c
void TIM2_IRQHandler(void) // Периодическое прерывание, частота 500 Гц
{
 // Вычисление времени, которое кнопка находится в состояниях
 // "нажата" и "отпущена"
 
 if (Flag) { // Кнопка нажата
  HighTime = HAL_GetTick() - TimeStamp;
 } else { // Кнопка отпущена
  LowTime = HAL_GetTick() - TimeStamp;
 }
}
```

Вычисленное время используется для реализации функции стирания текста, а также определения пауз между символами в слове и словами в сообщении.
Блок-схема алгоритмов вышеперечисленного функционала:

<p align="center" width="100%">
    <img width="80%" src="./Images/Pasted%20image%2020240104161022.png">
</p>

### Организация вывода на светодиодный модуль P10

Модуль P10 разделён на 4 зоны сканирования по 128 светодиодов в каждой. На рисунке ниже зоны обозначены цветами.

<p align="center" width="100%">
    <img width="100%" src="./Images/Pasted%20image%2020240105000601.png">
</p>

Каждая зона управляется 16 8-битными сдвиговыми регистрами с защелкой. Светодиоды подключены по матричной схеме, катоды
На принципиальной схеме (ниже) видно, что для того, чтобы зажечь светодиод, необходимо, чтобы на выходе регистра (D1-D16), подключенного к катоду этого светодиода был логический ноль, а также должен быть открыт соответствующий нужной зоне MOSFET-транзистор (VT1-VT4).

<p align="center" width="100%">
    <img width="100%" src="./Images/Pasted%20image%2020240105000721.png">
</p>

Таким образом, управлять зоной сканирования модуля можно последовательно загружая данные в сдвиговые регистры (вход R), затем перенося их в регистры хранения путем активации защелки (вход SCLK). Текущая зона выбирается комбинацией логических уровней на входах модуля A и B. Вход OE используется для разрешения работы матрицы, а также управления её яркостью путем подачи ШИМ-сигнала на этот вход.

# Лазарев Александр. КМБО-03-22. ВАРИАНТ 16

library("lmtest")
library("GGally")
library("car")

data = na.omit(swiss)

data
summary(data)
ggpairs(data)

# Проверим отсутствие линейной зависимости между регрессорами
model1 = lm(Agriculture~Catholic+Fertility, data)
model2 = lm(Catholic~Agriculture+Fertility, data)
model3 = lm(Fertility~Agriculture+Catholic, data)

summary(model1)
# R^2~0.16
summary(model2)
# R^2~0.25
summary(model3)
# R^2~0.21

# R^2 во всех случаях относительно низкий, звездочек всегда мало. 
# Из этого можно сделать предположение о том, что регрессоры
# линейно независимы. Чтобы убедиться, посмотрим на R^2 при
# добавлении регрессоров в нужную зависимость:

model01 = lm(Examination~Catholic, data)
summary(model01)
# R^2~0.31, имеется слабая зависимость
model02 = lm(Examination~Catholic+Agriculture, data)
summary(model02)
# R^2~0.56, модель стала значительно лучше
model0 = lm(Examination~Catholic+Agriculture+Fertility, data)
summary(model0)
# R^2~0.66, еще один заметный прирост, модель стала заметно лучше и
# может считаться хорошей, однако Сatholic имеет всего 1 звездочку.
# Это говорит о том, что религиозная принадлежность не так сильно
# влияет на результаты экзамена, как Agriculture и Fertility (по 3 звезды)

# Попробуем исключить Catholic: 
model00 = lm(Examination~Agriculture+Fertility, data)
summary(model00)
# R^2 снизился всего на 2% (~0.64), значит Catholic все-таки можно
# исключить. В итоге получаем искомую хорошую зависимость.

# Попробуем ввести в нашу зависимость логарифмы от регрессоров
# Для этого сначала исследуем их на линейную зависимость от исходных данных.

model1 = lm(log(Agriculture)~Agriculture, data)
summary(model1)
# R^2~0.77
model2 = lm(log(Fertility)~Fertility, data)
summary(model2)
# R^2~0.98
# Зависимости сильные, значит логарифмами нужно заменять исходные регрессоры.
# Иначе будет линейная зависимость.

model01 = lm(Examination~I(log(Agriculture))+Fertility, data)
summary(model01)
# R^2~0.61
model02 = lm(Examination~Agriculture+I(log(Fertility)), data)
summary(model02)
# R^2~0.62
# Введение не дало желанных результатов, R^2 уменьшился во всех случах. К тому
# же выросли стандартные ошибки. Exmination куда лучше зависит от исходных
# регрессоров нежели от их логарифмов, значит вводить их не требуется.

# Проделаем то же самое с попарными произведениями регрессоров
model1 = lm((Agriculture^2)~Agriculture, data)
summary(model1)
# R^2~0.94 => следует лишь попытаться использовать квадрат ВМЕСТО исходного.
model2 = lm((Fertility^2)~Fertility, data)
summary(model2)
# R^2~0.94 => следует лишь попытаться использовать квадрат ВМЕСТО исходного.
model3 = lm((Agriculture*Fertility)~Fertility+Agriculture, data)
summary(model3)
# R^2~0.94 => следует лишь попытаться использовать произведение ВМЕСТО исходных.

# Рассмотрим все варианты:
model03 = lm(Examination~Fertility+I(Agriculture^2), data)
summary(model03)
# R^2~0.61
model04 = lm(Examination~Agriculture+I(Fertility^2), data)
summary(model04)
# R^2~0.65
model05 = lm(Examination~I(Agriculture*Fertility), data)
summary(model05)
# R^2~0.56

# Из всех пролученных моделей не уменьшился R^2, не возрос p-value и не
# возрасла стандартная ошибка лишь у модели 04. В ней наоборот, слегка
# вырос R^2, p-value уменьшился на порядок (у Fertility), а стандартная
# ошибка уменьшилась на 2 порядка. Такая зависимость сильнее той, в которой
# участвует простой столбец Fertility. Значит лучшей моделью является
# model04 = lm(Examination~Agriculture+I(Fertility^2), data)

# Найдем доверительные интервалы для всех коэффициентов регрессоров.
# Количество измерений в обучающей выборке 44, рассчитано 3 коэффициента.
# Число степеней свободы в модели 44 - 3 = 41. Для такого числа степеней
# свободы и p = 95% рассчитаем значение t-критерия Стьюдента:

t_critical = qt(0.975, df = 41)
t_critical
# t_critical~2.02

# Коэффициент перед Agriculture k1 = -0.19, ско = 0.03.
# Тогда доверительный интервал для коэффициента k1 имеет вид
# k1:[-0.19-2.02*0.03; -0.19+2.02*0.03], k1:[-0,25; -0,13]
# 0 не попадает в интерал, значит сразу отвергаем гипотезу о том, что этот
# коэффициент может быть равен 0, на уровне значимости 5%.

# Коэффициент перед I(Fertility^2) k2 = -0.0022, ско = 0.0004.
# Тогда доверительный интервал для коэффициента k2 имеет вид
# k2:[-0.0022-2.02*0.0004; -0.0022+2.02*0.0004], k2:[-0,003; -0,0014]
# 0 не попадает в интерал, значит сразу отвергаем гипотезу о том, что этот
# коэффициент может быть равен 0, на уровне значимости 5%.

# Найдем доверительный интервал для прогноза с регрессорами Agriculture=60,
# I(Fertility^2)=4225, p=95%
new.data = data.frame(Agriculture = 60, Fertility=65)
predict(model04, new.data, interval = "confidence")
# Прогноз: Examination~16.58, доверительный интервал [14.81; 18.36]
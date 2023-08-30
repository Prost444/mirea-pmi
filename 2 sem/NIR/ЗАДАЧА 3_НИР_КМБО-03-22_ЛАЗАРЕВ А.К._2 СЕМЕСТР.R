# Лазарев Александр. КМБО-03-22. ВАРИАНТ 16

# Загрузка необходимых библиотек
library("lmtest")
library("rlms")
library("dplyr")
library("GGally")
library("car")

# Загрузка данных из CSV-файла
data <- read.csv("r12i_os26b.csv")

# Использование функции glimpse для ознакомления с данными
glimpse(data)

# Выбор необходимых столбцов из данных и присвоение их переменной data2
data2 = select(data, hj13.2, h_age, hh5, h_educ, status, hj6.2, h_marst,
               hj1.1.2, hj23, hj24, hj6, hj32, hj60)

# Описание параметров:
# h_age - возраст
# hh5 - пол
# hj13.2 - зарплата
# h_marst - семейное положение
# h_educ - наличие высшего образования
# hj6.2 - длительность рабочей недели
# status - тип населенного пункта
# hj1.1.2 - удовл-ть
# hj23 - владеет ли государство компанией?
# hj24 - есть ли иностранные совладельцы?
# hj60 - все денежные поступления

# Замена неотмеченных данных на NA в соответствующих столбцах
data2$hj13.2[which(data2$hj13.2>99999990)] <- NA
data2$h_age[which(data2$hj_age>99999990)] <- NA
data2$hh5[which(data2$hj5>99999990)] <- NA
data2$h_educ[which(data2$h_educ>99999990)] <- NA
data2$status[which(data2$status>99999990)] <- NA
data2$hj6.2[which(data2$hj6.2>99999990)] <- NA
data2$h_marst[which(data2$h_marst>99999990)] <- NA
data2$hj1.1.2[which(data2$hj1.1.2>99999990)] <- NA
data2$hj23[which(data2$hj23>99999990)] <- NA
data2$hj24[which(data2$hj24>99999990)] <- NA
data2$hj6[which(data2$hj6>99999990)] <- NA
data2$hj60[which(data2$hj60>99999990)] <- NA

# Удаление строк, содержащих NA
data2 = na.omit(data2)

# Создаем новый столбец 'wed' и копируем в него значения столбца 'h_marst'
data2["wed"]=data2$h_marst

# Создаем еще три новых столбца 'wed1', 'wed2' и 'wed3' и копируем в них значения 'h_marst'
data2["wed1"]=data2$h_marst
data2["wed2"]=data2$h_marst
data2["wed3"]=data2$h_marst

# Заполняем значения 'wed1', 'wed2' и 'wed3' нулями
data2$wed1 = 0
data2$wed2 = 0
data2$wed3 = 0

# Заполняем значения 'wed1' единицами, если 'h_marst' равно '2' или '6'
data2$wed1[which(data2$wed=='2')] <- 1
data2$wed1[which(data2$wed=='6')] <- 1

# Заполняем значения 'wed2' единицами, если 'h_marst' равно '4' или '5'
data2$wed2[which(data2$wed=='4')] <- 1
data2$wed2[which(data2$wed=='5')] <- 1

# Заполняем значения 'wed3' единицами, если 'h_marst' равно '1'
data2$wed3[which(data2$wed=='1')] <- 1

# Преобразуем значения 'wed1', 'wed2' и 'wed3' в числовой формат
data2$wed1 = as.numeric(data2$wed1)
data2$wed2 = as.numeric(data2$wed2)
data2$wed3 = as.numeric(data2$wed3)

# Создаем линейную регрессию с зависимой переменной 'hj13.2' и независимыми переменными 'wed1', 'wed2', 'wed3'
model0 = lm(hj13.2~wed1+wed2+wed3, data2)

# Вычисляем VIF для модели
vif(model0)
# Все значения низкие (не превосходят 5), что говорит об отсутвтвии мультиколлинеарности.

# Создаем новый столбец 'sex' и копируем в него значения столбца 'hh5'
data2["sex"]=data2$hh5

# Заменяем значения 'sex' на 0, если 'hh5' равно '2' (женский пол) и на 1, если 'hh5' равно '1' (мужской пол)
data2$sex[which(data2$sex=='2')] <- 0
data2$sex[which(data2$sex=='1')] <- 1

# Преобразуем значения 'sex' в числовой формат
data2$sex = as.numeric(data2$sex)


# Создание дамми-переменной "status2" на основе переменной "status"
data2["status2"] = data2$status
data2["status2"] = 0 # Заполнение столбца "status2" нулями
# Если значение "status" равно 1 или 2, то значение "status2" равно 1
data2$status2[which(data2$status=='1' | data2$status=='2')] <- 1
data2$status2 = as.numeric(data2$status2) # Преобразование столбца "status2" в числовой формат

# Создание дамми-переменной "higher_educ" на основе переменной "h_educ"
data2["higher_educ"] = data2$h_educ
data2["higher_educ"] = 0 # Заполнение столбца "higher_educ" нулями
# Если значение "h_educ" равно 21,22 или 23 то значение "higher_educ" равно 1
data2$higher_educ[which(data2$h_educ=='21' | data2$h_educ=='22' | data2$h_educ=='23')] <- 1
data2$higher_educ = as.numeric(data2$higher_educ) # Преобразование столбца "higher_educ" в числовой формат

# Создание дамми-переменной "satisfy" на основе переменной "hj1.1.2"
data2["satisfy"] = data2$hj1.1.2
data2["satisfy"] = 0 # Заполнение столбца "satisfy" нулями
# Если значение "hj1.1.2" равно 1 или 2, то значение "satisfy" равно 1
data2$satisfy[which(data2$hj1.1.2=='1' | data2$hj1.1.2=='2')] <- 1 
data2$satisfy = as.numeric(data2$satisfy) # Преобразование столбца "satisfy" в числовой формат

# Создание дамми-переменной "state_owner" на основе переменной "hj23"
data2["state_owner"] = data2$hj23
data2["state_owner"] = 0 # Заполнение столбца "state_owner" нулями
data2$state_owner[which(data2$hj23=='1')] <- 1 # Если значение "hj23" равно 1, то значение "state_owner" равно 1
data2$state_owner = as.numeric(data2$state_owner) # Преобразование столбца "state_owner" в числовой формат

# Создание дамми-переменной "foreign_owner" на основе переменной "hj24"
data2["foreign_owner"] = data2$hj24
data2["foreign_owner"] = 0 # Заполнение столбца "foreign_owner" нулями
data2$foreign_owner[which(data2$hj24=='1')] <- 1 # Если значение "hj24" равно 1, то значение "foreign_owner" равно 1
data2$foreign_owner = as.numeric(data2$foreign_owner) # Преобразование столбца "foreign_owner" в числовой формат

# Создание дамми-переменной "subordinates" на основе переменной "hj6"
data2["subordinates"] = data2$hj6
data2["subordinates"] = 0 # Заполнение столбца "subordinates" нулями
data2$subordinates[which(data2$hj6=='1')] <- 1 # Если значение "hj6" равно 1, то значение "subordinates" равно 1
data2$subordinates = as.numeric(data2$subordinates) # Преобразование столбца "subordinates" в числовой формат

# Создание дамми-переменной "second_job" на основе переменной "hj32"
data2["second_job"] = data2$hj32
data2["second_job"] = 0 # Заполнение столбца "second_job" нулями
data2$second_job[which(data2$hj32=='1')] <- 1 # Если значение "hj32" равно 1, то значение "second_job" равно 1
data2$second_job = as.numeric(data2$second_job) # Преобразование столбца "second_job" в числовой формат

  

# Преобразование переменной "hj13.2" в стандартизированную форму и создание новой переменной "salary"
sal = as.numeric(data2$hj13.2)
data2["salary"] = (sal - mean(sal)) / sqrt(var(sal))

# Преобразование переменной "h_age" в стандартизированную форму и создание новой переменной "age"
age = data2$h_age
data2["age"]= (age - mean(age)) / sqrt(var(age))

# Преобразование переменной "hj6.2" в стандартизированную форму и создание новой переменной "dur"
dur = data2$hj6.2
data2["dur"] = (dur - mean(dur)) / sqrt(var(dur))

# Преобразование переменной "hj60" в стандартизированную форму и создание новой переменной "payments"
payments = data2$hj60
data2["payments"] = (payments-mean(payments)) / sqrt(var(payments))

# Использование функции glimpse для ознакомления с данными
glimpse(data2)



# Объявляем модель и запускаем регрессионный анализ
model1 = lm(salary~dur+wed1+wed2+wed3+age+sex+status2+higher_educ+
              satisfy+state_owner+foreign_owner+subordinates+payments, data2)
summary(model1)
# R^2~0.6501
# Сразу даляем wed1, wed2, wed3 из модели, так как у них очень высокие значения p-value
model2 = lm(salary~dur+age+sex+status2+satisfy+state_owner+
              foreign_owner+subordinates+payments, data2)
summary(model2) # R~0.6478
# Оцениваем мультиколлинеарность
vif(model2)
# У всех регрессоров коэффициент вздутия не превосходит 3, что говорит об отсутствии мультиколлиниарности

# Находим минимальные значения для dur, age и payments
min(data2$dur) # ~ -3.22 => для возведения в рациональную степень и взятия логарифма к значению надо прибавить 4
min(data2$age) # ~ -2.14 => для возведения в рациональную степень и взятия логарифма к значению надо прибавить 3
min(data2$payments) # ~ -0.98 => для возведения в рациональную степень и взятия логарифма к значению надо прибавить 1


# Начинаем вводить функции от регрессоров. Первым делом посмотрим на возведение в степень.
# Из-за большого числа возможных моделей (1000) запишем решение циклом и отберем лучшую модель по максимальному значению R^2
arr = list() # Реализуем словарь, в котором ключем выступает набор степеней регрессоров, а значением R^2
for (i in seq(0.1, 2, 0.1)) {
  for (j in seq(0.1, 2, 0.1)) {
    for (k in seq(0.1, 2, 0.1)) {
      formula = paste("salary ~ I((dur+4)^", i, ")+I((age+3)^", j, ")+
                       sex+status2+higher_educ+satisfy+state_owner+foreign_owner+subordinates+I((payments+1)^", k, ")", sep = "")
      model = lm(formula, data2)
      arr[[paste0(as.character(i),", ",as.character(j),", ",as.character(k))]] <- summary(model)$adj.r.squared
    }
  }
}
max(unlist(arr)) # R^2 ~ 0.6541
names(arr)[which.max(unlist(arr))] # Интересующая модель имеет степени 0.3, 2, 1.1

model = lm(salary~I((dur+4)^0.3)+I((age+3)^2)+sex+status2+higher_educ+
               satisfy+state_owner+foreign_owner+subordinates+I((payments+1)^1.1), data2)
summary(model)
vif(model)
# p-статистики отличные (по 3 зведочки у каждого регрессора), как и коэффициенты вздутия (максимальный = 1.233064)
# Мы получили лучшую модель. Далее попробуем другие функции.

# Так как при взятии логарифма значения вновь станут ненормализированными, их надо нормализировать.
data2["log_dur"] = (I(log(dur+4))-mean(I(log(dur+4)))) / sqrt(var(I(log(dur+4))))
data2["log_age"] = (I(log(age+3))-mean(I(log(age+3)))) / sqrt(var(I(log(age+3))))
data2["log_payments"] = (I(log(payments+1))-mean(I(log(payments+1)))) / sqrt(var(I(log(payments+1))))
model1 = lm(salary~log_dur+log_age+sex+status2+higher_educ+
               satisfy+state_owner+foreign_owner+subordinates+log_payments, data2)
summary(model1) # R^2 ~ 0.3, что очень мало. Логарифмы включать не требуется

# Произведения
model2 = lm(salary~I(dur*age)+sex+status2+higher_educ+
               satisfy+state_owner+foreign_owner+subordinates+payments, data2)
summary(model2) # R^2~0.64. Воспользуемся этим далее.

model3 = lm(salary~I(dur*payments)+sex+status2+higher_educ+
               satisfy+state_owner+foreign_owner+subordinates+age, data2)
summary(model3) # R^2~0.26

model4 = lm(salary~I(age*payments)+sex+status2+higher_educ+
               satisfy+state_owner+foreign_owner+subordinates+dur, data2)
summary(model4) # R^2~0.27

model5 = lm(salary~I(dur*age*payments)+sex+status2+higher_educ+
               satisfy+state_owner+foreign_owner+subordinates, data2)
summary(model5) # R^2~0.24
# Значения R^2 не превосходят найденне ранее, значит произведения включать не следует.

# R^2 может повыситься при комбинации функций. Рассмотри таую функцию, которая может привести к этому.
model6 = lm(salary~I((dur+4)^0.3*(age+3)^2)+sex+status2+higher_educ+
             satisfy+state_owner+foreign_owner+subordinates+I((payments+1)^1.1), data2)
summary(model6) # R^2 ~ 0.6489, значит комбинацию включать не следует

# Таким образом мы получили лучшую модель salary~I((dur+4)^0.3)+I((age+3)^2)+sex+status2+higher_educ+
#                                         satisfy+state_owner+foreign_owner+subordinates+I((payments+1)^1.1
# Проведем ее анализ:

# Коэффициент регрессора:
# I((dur + 4)^0.3) - положительный 
# I((age + 3)^2)  - отрицательный
# sex - положительный
# status2 - положительный
# higher_educ - положительный
# satisfy - положительный
# state_owner - отрицательный
# foreign_owner - положительный
# subordinates - положительный
# I((payments + 1)^1.1) - положительный

# Вывод о том, какие индивиды получают большую зарплату: большую зарплату получают молодые мужчины с продолжительной рабочей неделей, имеющие высшее 
# образование, проживающие в городе, удовлетворённые своей заработной платой, имеющие подчиненных. При этом, если иностранные фирмы и частники являются 
# совладельцами или владельцами Вашего предприятия, то это положительно сказывается на уровне зааботной платы. Если
# государство являются совладельцами или владельцами Вашего предприятия, то это отрицательно сказывается на уровне зааботной платы.
# Так же отмечу, что чем больше денег индивид получает в течении месяца (учитывется не только зарплату), тем больше у него зарплата.

# Проведем анализ выбранных из варианта подмножеств.
data_unhappy_people = subset(data2, (wed3==1)&(higher_educ==0)) # Никогда не состоявшие в браке индивиды без высшего образования :(
model_unhappy_people = lm(salary~I((dur+4)^0.3)+I((age+3)^2)+sex+status2+higher_educ+
                        satisfy+state_owner+foreign_owner+subordinates+I((payments+1)^1.1), data_unhappy_people)
summary(model_unhappy_people) # R^2~0.72
# Уберем из модели незнучащие регрессоры.
model_unhappy_people = lm(salary~I((dur+4)^0.3)+sex+subordinates+I((payments+1)^1.1), data_unhappy_people)
summary(model_unhappy_people) # R^2~0.72
# Таким образом наибольшую зарплату из никогда не состоявшие в браке индивидов без высшего образования получают
# мужчины, имеющие подчиненных с высокой продолжительностью рабочей недели и высоким уровнем денежных поступлений за месяц.

data_happy_people = subset(data2, (status2==1)&(wed1==1)) # Городские жители, состоящие в браке :)
model_happy_people = lm(salary~I((dur+4)^0.3)+I((age+3)^2)+sex+status2+higher_educ+
                          satisfy+state_owner+foreign_owner+subordinates+I((payments+1)^1.1), data_happy_people)
summary(model_happy_people) # R^2 ~ 0.59
# Уберем из модели незнучащие регрессоры
model_happy_people = lm(salary~I((dur+4)^0.3)+I((age+3)^2)+sex+higher_educ+
                          state_owner+subordinates+I((payments+1)^1.1), data_happy_people)
summary(model_happy_people) # R^2 ~ 0.59
# Таким образом наибольшую зарплату из городских жителей, состоящих в браке получают молодые мужчины, с долгой рабочей неделей
# и высшим образованием, не работающие на государсьвенную компанию, имеющие подчиненных и высокий общий денежный доход.
# Лазарев Александр КМБО-03-22
# ВАРИАНТ 16

library("lmtest")
library("GGally")
library("car")

data = swiss
help(swiss)

data
plot(data$Fertility, data$Education)
plot(data$Examination, data$Education)

mean(data$Education)
# ~10.98 => образование на низком уровне
var(data$Education)
# ~92.46 => почти на порядок больше среднего => присутствует заметный разброс
sqrt(var(data$Education))
# ~9.61

mean(data$Fertility)
# ~70.14 => в среденем нормальная рождаемость
var(data$Fertility)
# ~156.04 => всего в 2 раза больше среднего => разброс не велик
sqrt(var(data$Fertility))
# ~12.49

mean(data$Examination)
# ~16.49 => низкий процент получения высшего балла
var(data$Examination)
# ~63.65 => более чем в 3 раза больше среднего => присутствует заметный разброс
sqrt(var(data$Examination))
# ~7.98


model_ed_fer = lm(Education~Fertility, data)
model_ed_fer
summary(model_ed_fer)
# education = -0.51*fertility + 46.82
# имеется явная (хорошие коэфициенты, p-value: 3.659e-07, у каждого 
# коэффициента по 3 звездочки) нисходящая зависимость,
# а значит, при большей рождаемости, образование снижается
# При этом R^2~0.43, модель нормально объясняет отклонения от
# среднего значения.


model_ed_ex = lm(Education~Examination, data)
model_ed_ex
summary(model_ed_ex)
# education = 0.84*Examination - 2.90
# имеется восходящая зависимость (p-value: 4.811e-08, коэффициент а
# имеет 3 звездочки), что логично, чем лучше результаты
# экзамена, тем выше качество образования. Однако коэфицент b
# представлен не точно (вообще без звездочек), и R^2~0.48, что
# говорит о том, что не все отклонения могут быть точно предсказаны.
library(ggplot2)

ggplot(data, aes(x = Fertility, y = Education)) +
  geom_point() +
  geom_smooth(method = "lm", se = FALSE, color = "red") +
  labs(x = "Fertility", y = "Education") +
  ggtitle("Scatterplot of Education vs. Fertility with Regression Line")
ggplot(data, aes(x = Examination, y = Education)) +
  geom_point() +
  geom_smooth(method = "lm", se = FALSE, color = "blue") +
  labs(x = "Examination", y = "Education") +
  ggtitle("Scatterplot of Education vs. Examination with Regression Line")


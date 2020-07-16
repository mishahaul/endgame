# имя библиотеки
NAME = endgame

# папка с исходниками
SRC_DIR = src

# папка, куда будут складываться объектные файлы
OBJ_DIR = obj

# папка с хедерами
INC_DIR = inc

SDL = -F resource/framework -I resource/framework/SDL2.framework/SDL2 -I resource/framework/SDL2_image.framework/SDL2_image

# сделать список всех файлов в папке src, которые имеют расширение .с
# формат каждой записи src/<file_name>.c
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# добавить префикс obj/ к каждой записи из SRC_FILES, в который убрали
# путь (до notdir src/<file_name>.c, после <file_name>.c) и изменить
# расширение с .с на .о
# результат выполнения src/<file_name>.o  --> obj/<file_name>.o
# о всех манипуляциях с именами можно почитать тут
# https://www.gnu.org/software/make/manual/html_node/File-Name-Functions.html
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC_FILES:%.c=%.o)))

# сделать список всех файлов в папке inc, которые имеют расширение .h
# формат каждой записи inc/<file_name>.h
INC_FILES = $(wildcard $(INC_DIR)/*.h)

# компилятор для создания o-файлов
CC = clang

# флаги для компилятора (добабить префикс -W ко всем записям после
# запятой)
CFLAGS = -std=c11 $(addprefix -W, all extra error pedantic) -g \

SDL_FLAGS = -rpath resource/framework -framework SDL2 \
		-framework SDL2_image \
		-I resource/framework/SDL2_image.framework/Headers \
		-framework SDL2_mixer \
		-I resource/framework/SDL2_mixer.framework/Headers \
		-framework SDL2_ttf \
		-I resource/framework/SDL2_ttf.framework/Headers \

# архиватор для формирования библиотеки из о-файлов
AR = ar

#флаги для архиватора
AFLAGS = rcs

MKDIR = mkdir -p
RM = rm -rf

# главная цель, при ее вызове вызывается цель libmx.a
all: $(NAME)

# цель зависит от времени создания o-файлов, т.е если находится какой-то
# о-файл время последней модификации которого позже, чем время последней
# модификации файла libmx.a, то будут запущены инструкции в этой цели
# $@ - переменная, которая означает имя цели
# $^ - переменная, которая означает все зависимости текущей цели
# просто для красивой оригинальной записи ;)
$(NAME): $(OBJ_FILES)
	@$(CC) $(CFLAGS) $^ -o $@ -I $(INC_DIR) $(SDL_FLAGS) $(SDL)
	@printf "\r\33[2K$@\t \033[32;1mcreated\033[0m\n"

# перед компиляцией o-файлов создаем папку obj, про пайпы в мейкафайле
# https://www.gnu.org/software/make/manual/html_node/Prerequisite-Types.html
$(OBJ_FILES): | $(OBJ_DIR)

# сравниваем время последней модификации по одному каждого о-файла
# с с-файлом c точно таким же именем и h-файлами. Если дата последней
# модицикации o-файла раньше, то будет перекомпилирован только этот
# с-файл и библиотека будет пересобрана. в случае с h-файлом будут
# перекомпилированы все c-файлы, т.к. это изменение касается каждого
# файла
# $< - переменная, которая означает имя первой зависимости цели
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_FILES)
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR) $(SDL)
	@printf "\r\33[2K$(NAME)\033[33;1m\t compile \033[0m$(<:$(SRC_DIR)/%.c=%)\r"

# создается папка obj
$(OBJ_DIR):
	@$(MKDIR) $@

# удаляем папку с о-файлами
clean:
	@$(RM) $(OBJ_DIR)
	@printf "$(OBJ_DIR) in $(NAME)\t \033[31;1mdeleted\033[0m\n"

# полностью удаляем результат работы мейкфайла
uninstall:
	@$(RM) $(OBJ_DIR)
	@$(RM) $(NAME)
	@@printf "$(NAME)\t \033[31;1muninstalled\033[0m\n"

# удаляем и заново собираем библиотеку
reinstall: uninstall all

# .PHONY - это явное указание имен целей мейкфайла, например, если
# в папке будет файл clean и попытаться выполнить команду make clean,
# то make попытается выполнить файл clean, а не цель clean
.PHONY: all uninstall clean reinstall

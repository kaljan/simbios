#include "buttons.h"

struct button_list *btn_list;

int button_init(button_dsc_t **button, GPIO_TypeDef *gpio, int pin)
{
	if (button == 0) {
		return -1;
	}

	if (*button == 0) {
		if ((*button = malloc(sizeof(button_dsc_t))) == NULL) {
			return -1;
		}

		(*button)->gpio = gpio;
		(*button)->pinmask = (((uint32_t)1) << pin);
		(*button)->state = GPIO_HIGH;
		(*button)->signal = NONE;
		(*button)->hold_cnt = 0;
	}

	return 0;
}

int add_button_to_list(struct button_list **list,
	GPIO_TypeDef *gpio,	int pin)
{
	struct button_list *tmp;
	if (list == 0) {
		return -1;
	}

	if (*list != 0) {
		tmp = *list;
		while (*list != 0) {
			*list = (*list)->next;
		}
	}

	if ((*list = malloc(sizeof(struct button_list))) == NULL) {
		return -1;
	}

	if (button_init(&(*list)->button, gpio, pin) != 0) {
		return -1;
	}

	(*list)->next = NULL;

	*list = tmp;

	return 0;
}

/*
 * Механизм обработки нажатия на кнопку.
 *
 * Событие нажатия на кнопку -- это событие когда сначала на кнопку
 * нажали а потом её отпустили.
 *
 * Сделать так, что бы когда одна из кнопок зажата другие кнопки
 * не обрабатывались.
 *
 * Работать будет примерно следующим образом
 *
 * 1. Сначала дожидаемся момента когда кнопка будет нажата.
 *    т.е. сначала кнопка была нажата, а потом её отпустили.
 *
 * 2. После этого запускается таймер, и идёт ожидание когда кнопка
 *    будет отпущена.
 *
 */
int button_handler(button_dsc_t *button)
{
	GPIO_State state = GPIO_Debounce(button->gpio, button->pinmask);

	if (button->state == GPIO_HIGH) {
		if ((state == GPIO_LOW)) {
			button->state == GPIO_LOW;
		}
		return 0;
	}

	if (state == GPIO_HIGH) {
		button->state = GPIO_HIGH;
		button->signal = PRESSED;
		return 0;
	}

	button->hold_cnt++;
	if (button->hold_cnt > BUTTON_HOLD_MAX) {
		button->signal = HOLD;
	}

	return 0;
}

/*
 * Эта функция ищет кнопку на которой произошло какое-либо событие.
 *
 * Если ни на одной кнопке не произошло события. то возвращается NULL
 *
 * Если хоть на одной кнопке произошло событие, то возвращается
 * указатель на дескриптор кнопки, на которой произошло событие.
 *
 */
button_dsc_t *get_button(struct button_list *list)
{
	button_dsc_t* btn = NULL;

	while (list != 0) {
		button_handler(list->button);
		if (list->button->state == GPIO_LOW) {
			btn = list->button;
			break;
		}
		list = list->next;
	}
	return btn;
}

/*
 * Обработчик кнопок. его нужно будет поместить в systick
 *
 */

int buttons_process(void)
{
	static button_dsc_t* btn == NULL;

	if (btn == NULL) {
		btn = get_button(btn_list);
		return 0;
	}

	button_handler(btn);

	return 0;
}

/*
 * Инициализация обработчика кнопок.
 */
int buttons_init(void)
{
	return 0;
}

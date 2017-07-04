#include "buttons.h"


struct button_list {
	button_dsc_t *button;
	struct button_list *next;
};

int button_init(button_dsc_t **button, GPIO_TypeDef *gpio,
	int pin, button_callback bclbk)
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
		(*button)->button_event = RELEASED;
		(*button)->button_state = RELEASED;
		(*button)->callback = bclbk;
		(*button)->hold_cnt = 0;
	}

	return 0;
}

int add_button_to_list(struct button_list **list,
	GPIO_TypeDef *gpio,	int pin, button_callback bclbk)
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

	if (button_init(&(*list)->button, gpio, pin, bclbk) != 0) {
		return -1;
	}

	(*list)->next = NULL;

	*list = tmp;

	return 0;
}

int button_handler(button_dsc_t *button)
{
	return 0;
}

int buttons_process(void)
{
	return 0;
}

int buttons_init(void)
{
	return 0;
}

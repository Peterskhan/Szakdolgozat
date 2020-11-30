#include "df_gpio.h"

bool DF_GPIO::s_isrServiceInstalled = false;

DF_GPIO::DF_GPIO(uint8_t gpioNum, Direction direction, PullMode pullMode, TriggerType triggerType)
	: m_gpioNum(gpioNum), m_direction(direction), m_thisTask(nullptr)
{
	// Installing ISR service for GPIO input interrupts on the first instance
	if(!s_isrServiceInstalled) {
		gpio_install_isr_service(0);
		s_isrServiceInstalled = true;
	}

	// Creating GPIO configuration according to the parameters
	gpio_config_t config;
	config.mode = (gpio_mode_t) direction;
	config.pull_up_en   = pullMode == PullMode::PULLUP ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
	config.pull_down_en = pullMode == PullMode::PULLDOWN ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;
	config.intr_type    = (gpio_int_type_t) triggerType;
	config.pin_bit_mask = (0x01 << gpioNum);

	// Configuring the GPIO according to the configuration
	gpio_config(&config);

	// Checking if the GPIO is used in INPUT mode with triggers
	if(direction == Direction::INPUT && triggerType != TriggerType::DISABLED) {

		// Adding ISR handler for this specific GPIO
		gpio_isr_handler_add((gpio_num_t) gpioNum, interrupt_handler, (void*) this);
	}

	// Adding output port for GPIO used as input
	if(direction == Direction::INPUT) {
		m_ports.addOutputPort("out");

	}

	// Adding input port for GPIO used as output
	else {
		m_ports.addInputPort("in");
	}
}

void DF_GPIO::process()
{
	// Saving the handle to the task execution this component
	m_thisTask = xTaskGetCurrentTaskHandle();

	// Indefinitely waiting for events to happen
	while(true) {

		// GPIO input mode: suspend execution until signaled from ISR
		if(m_direction == Direction::INPUT) {

			// Suspending execution until signaled by ISR
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

			// Creating output message
			Node message("root", (int) gpio_get_level((gpio_num_t) m_gpioNum));

			// Sending the output message
			m_ports["out"].send(message);
		}

		// GPIO output mode: suspend until a message arrives to change the output
		else if(m_direction == Direction::OUTPUT) {

		}
	}
}

void DF_GPIO::interrupt_handler(void* params) {

	// Send notification to the task blocked on waiting for GPIO event
	vTaskNotifyGiveFromISR(static_cast<DF_GPIO*>(params)->m_thisTask, nullptr);

	// Returning from the ISR context to the next task to be served
	portYIELD_FROM_ISR();
}

# Sample Project Proposal – SmartFit Station

**Abstract**

This project proposes the design and implementation of an embedded measurement system capable of capturing human body width measurements at selected body heights (e.g., hip and shoulder). The goal is to explore precise distance measurement, signal timing, noise handling, and hardware integration using an STM32 microcontroller and ultrasonic sensors.

The system will sequentially trigger multiple ultrasonic sensors to measure distances and compute body width based on known geometry. Measurements will be processed using timer-based pulse width capture and filtered to improve stability. Final results will be transmitted over USB serial to a host PC for display and logging.

---

**Hardware**

I have selected the **STM32 NUCLEO-G0B1RE** development board as the main controller. This platform provides access to high-resolution timers, GPIO, and USB serial communication, which are suitable for ultrasonic pulse-width measurement.

The sensing system will consist of two to four **HC-SR04 ultrasonic sensors**. Initial development and testing will focus on validating sensor behavior when measuring distances to a human subject wearing clothing. Full mechanical mounting on a fixed-width frame will be considered only after sensor performance has been verified.

A **custom PCB will be designed and fabricated** as part of this project. The PCB will act as an interface between the STM32 NUCLEO board and the ultrasonic sensors. It will handle sensor connections, power distribution, and signal conditioning, including protection or level shifting for the ultrasonic echo signals. The NUCLEO board will connect to the PCB using headers rather than loose jumper wires.

Basic **LED indicators** may be included on the PCB to provide visual feedback for power, sensor triggering, or measurement activity during testing and demonstration.

The system will be powered over USB, and no battery operation is planned for this project.

---

**Research**

There are several areas that will require research and experimentation before the design can be finalized:

- Configuration and limitations of STM32 timers for microsecond-level pulse-width measurement  
- Ultrasonic sensor behavior when measuring distances to clothing and soft surfaces  
- Crosstalk effects when multiple ultrasonic sensors are used in close proximity  
- Appropriate signal conditioning and PCB routing for accurate echo timing  
- Filtering strategies to improve measurement stability and repeatability  

Some trial-and-error testing will be required to determine optimal sensor placement, timing parameters, and filtering window sizes.

---

**Proof of Concept**

For the initial proof-of-life build, I will interface a single ultrasonic sensor to the STM32 through the custom PCB. The STM32 will trigger the sensor, measure the echo pulse width using a hardware timer, and transmit the calculated distance over USB serial.

Testing will focus on verifying reliable distance measurements under realistic conditions, including measurements taken with clothing present. Once single-sensor operation is validated against known distances, the design will be expanded to support a pair of sensors for width computation.

---

**Stretch Goals**

If time permits, additional features may be implemented, including support for multiple measurement heights, multi-sample averaging triggered by a user command, structured serial data output, and additional LED status indicators. A simple PC-side script for logging or visualizing measurements may also be developed. These features are considered optional and are not required for core project success.

---

**Final Build**

The final build will consist of the STM32 NUCLEO board connected to a custom-designed PCB, with ultrasonic sensors connected via headers. Mechanical mounting will be minimal during early testing, with emphasis placed on reliable sensing, PCB design, and firmware functionality.

All power will be supplied via USB. No enclosure is planned for this project, as the focus is on embedded measurement, PCB design, and firmware behavior rather than portability or consumer packaging.

---

**Schedule**

**Week 1–2**
- Finalize system requirements  
- Research ultrasonic timing and STM32 timer operation  

**Week 3–4**
- Single-sensor proof-of-life testing  
- Timer configuration and echo pulse measurement  

**Week 5–6**
- Custom PCB schematic and layout  
- PCB review and fabrication submission  

**Week 7–8**
- PCB assembly and bring-up  
- Filtering, timeout handling, and LED indicators  

**Final Weeks**
- Multi-sensor integration  
- Repeatability testing  
- Documentation and demonstration preparation  


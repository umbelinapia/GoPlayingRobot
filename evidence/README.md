# Evidence

Supporting media for the Go-playing robot project: CAD renders, assembly photos, wiring diagrams, GUI screenshots, and design/flow diagrams. Imported from the local "supporting media content" folder.

| File | Label / Description |
|---|---|
| `wiring_diagram_arduino_servos.png` | Wiring schematic: Arduino Uno on a breadboard powered by USB (serial) and a separate 6V supply, driving three servos (Joint 1, Joint 2, Joint 3) via PWM, with a color-coded legend for ground, power, PWM signal, and serial lines. |
| `labeled_arduino_stripboard_wiring.jpg` | Photo of the real Arduino Uno wired to a stripboard, annotated with labels for Motors, PWM Signals, Laptop (USB), Ground, Power, and Serial Communication. |
| `system_block_diagram.png` | Hardware architecture block diagram: Python GUI sends serial commands over USB to the Arduino Uno, which sends PWM signals to the SCARA robot's three servos (Servo 1, Servo 2, Gripper servo); a separate power supply (gated by an emergency switch) powers the robot, which acts on the Go environment (board, bowls, stones). |
| `software_dataflow_diagram.png` | Software data-flow diagram showing three stages: (1) Graphical User Interface user input, (2) Arduino control system (command parsing → task logic → coordinate mapping → inverse kinematics → joint angles), (3) Output actuation to Servo 1, Servo 2, and the gripper servo. |
| `command_flowchart.png` | Logic flowchart for processing a `letter-number-letter` command (e.g. `B3P`), branching on the trailing R (release/place stone) or P (pick stone) code and on space/stone availability to the left or right, ending in `return to standby`. |
| `cad_dimensioned_cross_section.png` | Dimensioned CAD cross-section drawing of the arm/base assembly with key measurements in mm (110, 90, 140, 44, 22, 32, 10.5). |
| `robot_isometric_view.png` | Isometric photo of the fully assembled robot: cylindrical rotating base, boom arm with pulley/belt drive, gripper servo with magnetic end-effector, board with rows of stone slots, and a battery pack in a drawer under the base. |
| `robot_top_view_1.png` / `robot_top_view_2.png` | Top-down photos of the assembled robot in two arm positions, showing the rotating base disc, timing belt/pulley drive along the boom, and the blue micro servo end-effector positioned over the stone board. |
| `pulley_belt_closeup.png` | Two close-up shots of the GT2 timing pulley and belt at the arm's rotating joint. |
| `gripper_servo_horn_positions.png` | Side-by-side photos of the gripper servo horn at three rotation angles, showing the end-effector's release / neutral / pick-up positions. |
| `labeled_component_overview.png` | Annotated photos identifying the robot's main parts: Casing, Bowl, Stone, Timing belt and pulley, End-effector, and Power switch. |
| `low position.png` / `mid position.png` / `high position.png` | SolidWorks cross-section views of the vertical plunger/rod at its three stroke positions (fully lowered, mid-stroke, fully raised) within its housing. |
| `side slit view in casing.png` | SolidWorks assembly cross-section (side view) showing the full internal layout: servo stack, drive shaft, belt/pulley, and linkage inside the casing. |
| `go_gui_empty_board.png` | Screenshot of the "Responsive 5x5 Go Board" Python GUI with an empty board and the Arduino connection shown as ENABLED / "Sending to robot". |
| `go_gui_board_with_stones.png` | Screenshot of the same GUI mid-game, with black and white stones placed on the board and the Arduino link still ENABLED. |
| `target_coordinates_plot.png` | Plotted coordinate map (mm) of board target points, bowl target points, and the origin (0,0), with board and side-bowl outlines, used for the robot's placement coordinate calibration. |

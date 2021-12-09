# RESET has NO code on PIC!
# serial reset event sends a rs-232 BREAK which is connected
# through a filter to MCLR pin
#
# Python_TX_pin--(100ohm)--(+Schottky Diode-)------>(target MCLR pin)
#                                             |
#                                     (10uf)------(1kohm)
#                                        |           |
#                                        -------------
#                                             |
#                                         (PIC gnd)
#

import PySimpleGUI as sg
import serial

# on Windows the device will be 'COMx'
device = None
ser = serial.Serial(device, 38400, timeout=0.001)

sg.theme('DarkAmber')

# Encode Char
def encode_char(c):
    m = 0
    if 'U' in c.upper():
        m |= 1
    elif 'D' in c.upper():
        m |= 2
    elif 'F' in c.upper():
        m |= 4
    elif 'B' in c.upper():
        m |= 8
    elif 'R' in c.upper():
        m |= 16
    elif 'L' in c.upper():
        m |= 32
  
    if m == 0:
        return 0

    if '\'' in c:
        m |= 64
    elif '2' in c:
        m |= 128

    return m

font_spec = 'Courier 24 bold'
heading_color = '#3684ad'
canvas_color = '#3684ad'
groove_width = 2
layout = [  # Header
            [sg.Text('Rubot', font = 'Helvetica 32 bold underline', background_color = canvas_color)],

            # User Move Inputs
            [sg.Frame('Move Input', font = 'Helvetica 12 bold', background_color = canvas_color, title_location = 'n', element_justification = 'center', expand_x = True, border_width = groove_width,
               layout = [
                         [sg.Text('Seperate each move with a space. Half moves are allowed.', font = 'Helvetica 12', background_color = canvas_color)],
                         #[sg.Multiline(default_text = 'Seperate each move with a space. Half moves are allowed.', key = 'moves', size = (50, 3), autoscroll = True)],
                         [sg.Input(default_text = '', key = 'user_moves', size = (50, 3), expand_x = True)],
                         [sg.Button('Solve', key = 'solve_start', font = 'Helvetica 12')],
                        ]
            )],

            # TFT Controls - Step Through Moves
            [sg.Frame('TFT Controls', font = 'Helvetica 12 bold', background_color = canvas_color, title_location = 'n', element_justification = 'center', expand_x = True, border_width = groove_width,
               layout = [
                         [
                          sg.Button('<', key = 'prev', font = 'Helvetica 12', s = (2)), 
                          sg.Text('Move', font = 'Helvetica 12 bold', background_color = canvas_color),
                          sg.Button('>', key='next', font='Helvetica 12', s = (2))
                         ]
                        ]
               )],

            # Automatic Scrambling
            # [sg.Frame('Auto Scramble', font = 'Helvetica 12 bold', background_color = canvas_color, title_location = 'n', element_justification = 'center', expand_x = True, border_width = groove_width,
            #    layout = [
            #              [
            #               sg.Button('Scramble', key = 'scramble', font = 'Helvetica 12 bold'),
            #               sg.Spin(values = [x for x in range (33)], initial_value = 0, key = 'scramble_num', font = 'Helvetica 12 bold', s = 2, expand_y = True,)
            #              ],
            #              [sg.Text('Scramble Move Set', font = 'Helvetica 12 bold', background_color = canvas_color)],
            #              [sg.Multiline('', key = 'console', size = (50,10), autoscroll = True, enable_events = False)],
            #             ]
            # )],
            
            # Sys Controls
            [sg.Frame('System Controls', font = 'Helvetica 12 bold', background_color = canvas_color, title_location = 'n', element_justification = 'center', expand_x = True, border_width = groove_width,
               layout = [
                         [
                          # sg.Button('RESET PIC', key = 'rtg', font = 'Helvetica 12'),
                          sg.Button('EXIT', key = 'Exit', font='Helvetica 12', bind_return_key = False)
                         ],
                         # [
                         #  sg.Checkbox('reset_enable', key='r_en',font='Helvetica 8', enable_events=True),
                         #  sg.Button('RESET PIC', key='rtg', font='Helvetica 8')
                         # ]
                        ]
               )],
         ]

# change the colors in any way you like.
sg.SetOptions(background_color='#3684AD',
       text_element_background_color='#9FB8AD',
       text_color = '#fc0328',
       element_background_color='#475841',#'#9FB8AD',
       scrollbar_color=None,
       input_elements_background_color='#9FB8AD',#'#F7F3EC',
       progress_meter_color = ('green', 'blue'),
       button_color=('white','#475841'),
       )

# Create the Window
window = sg.Window('Rubot Interface', layout, location=(0,0),
                    return_keyboard_events=True, use_default_focus=True,
                    element_justification='c', finalize=True)

# Bind the realtime button release events <ButtonRelease-1>
# https://github.com/PySimpleGUI/PySimpleGUI/issues/2020
#window['pushbut01'].bind('<ButtonRelease-1>', 'r')
#window['pushbut02'].bind('<ButtonRelease-1>', 'r')

# Event Loop to process "events"
# event is set by window.read
event = 0
#
#  button state machine variables
button_on = 0
button_which = '0'
#
#
while True:

    # time out paramenter makes the system non-blocking
    # If there is no event the call returns event  '__TIMEOUT__'
    event, values = window.read(timeout=20) # timeout=10
    #
    #print(event)  # for debugging
    # if user closes window using windows 'x' or clicks 'Exit' button
    if event == sg.WIN_CLOSED or event == 'Exit':
        ser.close()
        break

    if event == 'solve_start':
        # Grab the given user moves
        move_string = window.Element('user_moves').get()

        # Convert move string to a String array
        char_array = move_string.split()
        print(char_array)

        # Convert char_array to an encoded array
        encoded_array = []
        for c in char_array:
            m = encode_char(c)
            if m:
                encoded_array.append(m)

        # Encode all move data to a char buffer
        num_moves = len(encoded_array) #num moves
        s = 's'.encode() + (num_moves).to_bytes(1, 'big') + bytes(encoded_array) + '\r'.encode()
        assert len(s) == 3 + num_moves

        # zero the input field
        window['user_moves'].update('')

        #Send solve data to PIC
        ser.write((s))

    # User requests next move
    if event == 'next':
        #send the next move buffer to PIC
        ser.write('n\r'.encode())

    # User requests previous move
    if event == 'prev':
        #send the previous move buffer to PIC
        ser.write('p\r'.encode())

# close port and Bail out
ser.close()
window.close()

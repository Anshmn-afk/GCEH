import cv2, mediapipe as mp, serial, time

# open serial port
arduino = serial.Serial('COM3', 9600, timeout=1)
time.sleep(2)

mp_hands = mp.solutions.hands
hands = mp_hands.Hands(min_detection_confidence=0.7)
mp_draw = mp.solutions.drawing_utils

def get_gesture(landmarks):
    y0 = landmarks[4].y    # thumb tip
    y1 = landmarks[8].y    # index tip
    y2 = landmarks[12].y   # middle tip
    y3 = landmarks[16].y   # ring tip
    y4 = landmarks[20].y   # pinky tip
    palm = landmarks[0].y

    fingers = [y1 < palm, y2 < palm, y3 < palm, y4 < palm]
    thumb_up = y0 < palm

    if all(fingers) and thumb_up: return "OPEN"
    if not any(fingers) and not thumb_up: return "FIST"
    if fingers[0] and not any(fingers[1:]): return "POINT"
    if thumb_up and not any(fingers): return "THUMB"
    return None

cap = cv2.VideoCapture(0)
last_sent = ""

while cap.isOpened():
    ret, frame = cap.read()
    if not ret: break

    rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    res = hands.process(rgb)
    gesture = None

    if res.multi_hand_landmarks:
        for handLms in res.multi_hand_landmarks:
            mp_draw.draw_landmarks(frame, handLms, mp_hands.HAND_CONNECTIONS)
            gesture = get_gesture(handLms.landmark)

    if gesture and gesture != last_sent:
        arduino.write((gesture + "\n").encode())
        last_sent = gesture
        print("Sent:", gesture)

    cv2.imshow("Gesture Control", frame)
    if cv2.waitKey(1) & 0xFF == 27: break  # ESC to quit

cap.release()
cv2.destroyAllWindows() 


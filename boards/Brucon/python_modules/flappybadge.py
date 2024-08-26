# Made originally for CampZone 2019 "i-Pane" by ForT3X
# Original app: https://hatchery.badge.team/projects/flappy_badge

import defines
import rgb
import buttons
import machine
from time import sleep

exit = 1
score = 0
width = 32
height = 8
playerY = 3.0
playerX = 6
playerVelocity = 0.0
playerJumpVelocity = 0.7
btnPressed = -1
gravity = 0.92
score = 0
currentHighscore = 0
highscoreDrawX = 0
scoreDrawX = 0
newHighscore = 0
pauseA = None

#initiële positie van de pijpen initiëren
pipeList = [[31, machine.random(1, 5)], [42, machine.random(1, 5)], [
    53, machine.random(1, 5)]]

ground_width = rgb.screenwidth
ground_height = rgb.screenheight - 8
ground_image = [0x008000FF] * ground_width * ground_height

#A knop afvangen en onnodige fix voor als er 2 events in 1 frame plaatsvinden
def input_A(pressed):
    global btnPressed
    global pauseA
    if pauseA == True:
        return
    if btnPressed == defines.BTN_A:
        print("Not handled")
    else:
        global playerVelocity
        playerVelocity = playerJumpVelocity
    print("Pressed")
    btnPressed = defines.BTN_A

#Render de speler pixel en elke pijp
def render():
    rgb.clear()
    rgb.pixel((100, 70, 150), (int(playerX), int(playerY)))
    for x in pipeList:
        drawPipe(x[0], x[1])

#Pijp tekenen op de desbetreffende X positie en een gat van 3 pixels maken om de gate waarde
def drawPipe(positionX, gate):
    for x in range(height):
        if x != gate-1 and x != gate and x != gate+1:
            rgb.pixel((140, 45, 40), (positionX, x))

#Score tekenen en afhankelijk van de score de positie en de duratie van de pauze aanpassen
def showScore():
    global score
    global currentHighscore
    global btnPressed
    global pauseA
    #Geen scoren dan gelijk terug naar de game
    if score != 0:
        rgb.disablecomp()
        rgb.clear()
        rgb.image(ground_image, pos=(0, 8), size=(ground_width, ground_height))

        checkHighscore(score)

        highscoreDrawX = 22
        if currentHighscore >= 10:
            highscoreDrawX = 19

        scoreDrawX = 5
        if score >= 10:
            scoreDrawX = 2

        if newHighscore:
            rgb.text(str(currentHighscore), (30, 120, 60),  (highscoreDrawX, 0))
            rgb.text(str(score), (30, 120, 60),  (scoreDrawX, 0))
        else:
            rgb.text(str(currentHighscore), (170, 10, 50),  (highscoreDrawX, 0))
            rgb.text(str(score), (90, 30, 90),  (scoreDrawX, 0))

        #Scheidingslijn tekenen
        for i in range(2):
            for j in range(8):
                rgb.pixel((100, 100, 100), (15+i, j))

        #Spullllll
        pauseA = True
        sleep(0.2)

        while btnPressed != defines.BTN_A:

            #Wait for A press to continue
            pauseA = None

            if btnPressed == defines.BTN_A:
                print("Breakout")
                break

        resetGame()
    else:
        resetGame()

#Check of de nieuwe score hoger is dan de huidige Highscore (Toekomstig update met lokaal opgeslagen highscore)
def checkHighscore(reachedScore):
    global newHighscore
    global currentHighscore

    if currentHighscore < reachedScore:
        newHighscore = 1
        currentHighscore = reachedScore
        machine.nvs_setint('flappyScore', 'highscore', reachedScore)
    else:
        newHighscore = 0
#data = machine.nvs_getint('my_namespace', 'my_keyname')

def loadHighscore():
    global currentHighscore
    currentHighscore = machine.nvs_getint('flappyScore', 'highscore')
    if currentHighscore == None:
        currentHighscore = 0
        machine.nvs_setint('flappyScore', 'highscore', 0)
    print(currentHighscore)

#Alles weer terugzetten naar begin waardes
def resetGame():
    global pipeList
    pipeList=[[31, machine.random(1, 5)], [42, machine.random(1, 5)], [53, machine.random(1, 5)]]
    global playerY
    playerY=3.0
    global playerX
    playerX=6
    global playerVelocity
    playerVelocity=0.0
    global playerJumpVelocity
    playerJumpVelocity=0.7  # 0.8
    global btnPressed
    btnPressed=-1
    global score
    score=0

# Button events registreren
buttons.register(defines.BTN_A, input_A)

# Highscore inladen vanuit de local storage
loadHighscore()

while exit != 0:  # Zolang er geen B gedrukt wordt run de loop

    # Logica

    # Input uitlezen en consume
    if btnPressed == defines.BTN_A:
        btnPressed=-1

    # Movement player aanpassen
    playerVelocity=playerVelocity * gravity
    playerVelocity=playerVelocity - 0.2
    playerY -= playerVelocity

    # Pijpen checken en eventueel weghalen/nieuwe spawnen
    for pipe in pipeList:
        pipe[0] -= 1
        if pipe[0] < 0:
            pipe[0] += 32
            pipe[1]=machine.random(1, 6)
        if pipe[0] == playerX:
            if int(playerY) >= pipe[1]-1 and int(playerY) <= pipe[1] + 1:
                # Pijp gehaald
                score += 1
            else:
                # Dood
                showScore()

    # Render
    render()
    sleep(0.06)

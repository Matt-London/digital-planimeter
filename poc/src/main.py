from estimators.estimators import greens_theorem_area

import pygame
import time


# Initialize Pygame
pygame.init()

# Colors
black = (0, 0, 0)
white = (255, 255, 255)

# Set screen dimensions
screen_width = 800
screen_height = 600
screen = pygame.display.set_mode((screen_width, screen_height))
screen.fill(white)

# Drawing variables
drawing = False
last_pos = None
DELAY_HZ = 100

DELAY_SEC = 1 / DELAY_HZ

points = []

font = pygame.font.SysFont("verdana", 12)  
position_update_event = pygame.USEREVENT + 1
pygame.time.set_timer(position_update_event, int(DELAY_SEC * 1000))

# Main loop
running = True
while running:
    # Write current position to the screen
    for event in pygame.event.get():
        if event.type == position_update_event: # timer event
            x, y = pygame.mouse.get_pos()
            pygame.draw.rect(screen, white, (0, 0, 80, 20))
            text = font.render(f"({x}, {y})", True, (0, 128, 0))
            screen.blit(text, (0, 0))

        if event.type == pygame.QUIT:
            running = False

        if event.type == pygame.MOUSEBUTTONDOWN:
            drawing = True
            last_pos = event.pos

        if event.type == pygame.MOUSEMOTION and drawing:
            pygame.draw.line(screen, black, last_pos, event.pos)
            last_pos = event.pos

        if event.type == pygame.MOUSEBUTTONUP:
            drawing = False
            last_pos = event.pos
            points.append(last_pos)

            # here we will calculate area and print it
            area = greens_theorem_area(points)
            points = []
            pygame.draw.rect(screen, white, (screen_width - 100, 0, screen_width - 20, 20))
            text = font.render(f"A={area}", True, (0, 128, 0))
            screen.blit(text, (screen_width - 100, 0))

    
    # Here we have last_pos which should be the "current" position in this refresh if we are drawing
    # so we can take that as a point and calculate
    if drawing:
        points.append(last_pos)

        time.sleep(DELAY_SEC)

    # Update display
    pygame.display.flip()

# Quit Pygame
pygame.quit()
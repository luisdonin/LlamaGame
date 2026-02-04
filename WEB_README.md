# Jack LLaman - Llaming Around! (Web Version)

A web-based recreation of the LlamaGame, originally built in C with SDL2. This version uses HTML5, CSS3, and JavaScript.

## How to Play

### Online
Simply open `index.html` in a modern web browser.

### Local Server
For best results, serve the game through a local web server:

```bash
# Using Python 3
python3 -m http.server 8000

# Using Python 2
python -m SimpleHTTPServer 8000

# Using Node.js http-server
npx http-server
```

Then visit `http://localhost:8000` in your browser.

## Controls

- **Space**: Start game / Restart after game over
- **Arrow Up** or **W**: Jump
- **P**: Pause/Resume game
- **ESC**: (Console log - can be extended for quit functionality)

## Game Features

- **Infinite Runner**: Jump over cacti as they come toward you
- **Score Tracking**: Your score increases the longer you survive
- **Increasing Difficulty**: The game speeds up as you play
- **Pause System**: Press P to pause and resume
- **Restart**: After game over, press Space to play again

## Technical Details

### Files
- `index.html` - Main HTML structure
- `style.css` - Game styling and layout
- `game.js` - All game logic and mechanics
- `images/` - Game sprites and assets

### Game Mechanics
- Canvas-based rendering (1080x720)
- Gravity and jump physics
- Collision detection
- Dynamic obstacle spawning
- State management (Init, Playing, Paused, Game Over)

## Credits

**Original C Game Developers:**
- J.L.B.DONIN
- LUIZ FELIPE NAGATANI RABELLO
- PEDRO GABRIEL MARINHO DA SILVA

**Web Conversion:**
- Recreated for web browsers using modern web technologies

## Original Project

This game is a web recreation of the original C/SDL2 project developed as a conclusion project for the introduction to programming course at UTFPR - Toledo.

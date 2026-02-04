// Game constants based on C code
const GAME_WIDTH = 1080;
const GAME_HEIGHT = 720;

const LLAMA_X = 200;
const LLAMA_Y = 192;
const LLAMA_WIDTH = 68;
const LLAMA_HEIGHT = 68;

const CACTUS_START_X = 900;
const CACTUS_Y = 170;
const CACTUS_WIDTH = 95;
const CACTUS_HEIGHT = 100;

const FLOOR_Y = 260;
const FLOOR_HEIGHT = 460; // GAME_HEIGHT - FLOOR_Y
const GRASS_HEIGHT = 60;

const JUMP_SPEED = 20;
const GRAVITY = 5;
const MIN_Y = 10;

// Game states
const GameState = {
    INIT: 'initscreen',
    PLAYING: 'game',
    PAUSED: 'pause',
    GAME_OVER: 'gameOver'
};

// Main Game class
class LlamaGame {
    constructor() {
        this.canvas = document.getElementById('gameCanvas');
        this.ctx = this.canvas.getContext('2d');
        this.scoreDisplay = document.getElementById('score-display');
        
        // Game state
        this.currentState = GameState.INIT;
        this.score = 0;
        this.startTime = 0;
        this.speed = 2;
        this.acceleration = 0;
        this.maxSpeed = 15; // Cap maximum speed
        
        // Llama properties
        this.llama = {
            x: LLAMA_X,
            y: LLAMA_Y,
            width: LLAMA_WIDTH,
            height: LLAMA_HEIGHT,
            falling: false,
            jumpSpeed: 0
        };
        
        // Optimized cactus management
        this.activeCactus = {
            x: CACTUS_START_X,
            y: CACTUS_Y,
            width: CACTUS_WIDTH,
            height: CACTUS_HEIGHT
        };
        
        // Images
        this.images = {};
        this.imagesLoaded = 0;
        this.totalImages = 0;
        
        // Input
        this.keys = {};
        
        // Frame timing for smooth movement
        this.lastFrameTime = 0;
        
        // Bind methods
        this.update = this.update.bind(this);
        this.render = this.render.bind(this);
        this.gameLoop = this.gameLoop.bind(this);
        
        // Load assets and start
        this.loadAssets();
        this.setupInput();
    }
    
    resetCactus() {
        // Reset single cactus to starting position with random offset
        this.activeCactus.x = CACTUS_START_X + Math.random() * 500;
        this.activeCactus.y = CACTUS_Y;
    }
    
    loadAssets() {
        const imageList = {
            llama: 'images/llamma-main.png',
            cactus: 'images/cactus.png',
            gameOver: 'images/LLAME-OVER.png',
            sky: 'images/sky.png',
            startScreen: 'images/startscreen.png',
            pause: 'images/pause.png',
            logo: 'images/logo.png',
            studio: 'images/studio.png',
            restartLlama: 'images/gameoverllama.png'
        };
        
        this.totalImages = Object.keys(imageList).length;
        
        for (let key in imageList) {
            const img = new Image();
            img.onload = () => {
                this.imagesLoaded++;
                if (this.imagesLoaded === this.totalImages) {
                    this.startGame();
                }
            };
            img.onerror = () => {
                console.error(`Failed to load image: ${imageList[key]}`);
                this.imagesLoaded++;
                if (this.imagesLoaded === this.totalImages) {
                    this.startGame();
                }
            };
            img.src = imageList[key];
            this.images[key] = img;
        }
    }
    
    setupInput() {
        window.addEventListener('keydown', (e) => {
            this.keys[e.key] = true;
            
            // Handle specific keys
            if (e.key === 'Escape') {
                // Could add exit functionality
                console.log('ESC pressed');
            } else if (e.key === ' ') {
                e.preventDefault();
                if (this.currentState === GameState.INIT) {
                    this.currentState = GameState.PLAYING;
                    this.resetGame();
                } else if (this.currentState === GameState.GAME_OVER) {
                    this.currentState = GameState.PLAYING;
                    this.resetGame();
                }
            } else if (e.key === 'p' || e.key === 'P') {
                if (this.currentState === GameState.PLAYING) {
                    this.currentState = GameState.PAUSED;
                } else if (this.currentState === GameState.PAUSED) {
                    this.currentState = GameState.PLAYING;
                }
            }
        });
        
        window.addEventListener('keyup', (e) => {
            this.keys[e.key] = false;
        });
    }
    
    resetGame() {
        this.score = 0;
        this.startTime = Date.now();
        this.speed = 2;
        this.acceleration = 0;
        this.llama.y = LLAMA_Y;
        this.llama.falling = false;
        this.llama.jumpSpeed = 0;
        this.lastFrameTime = 0;
        this.resetCactus();
    }
    
    startGame() {
        console.log('All assets loaded, starting game');
        this.gameLoop();
    }
    
    update(currentTime) {
        if (this.currentState !== GameState.PLAYING) {
            return;
        }
        
        // Calculate delta time for frame-independent movement
        if (!this.lastFrameTime) {
            this.lastFrameTime = currentTime;
        }
        const deltaTime = Math.min((currentTime - this.lastFrameTime) / 16.67, 2); // Cap at 2x for performance
        this.lastFrameTime = currentTime;
        
        // Update score based on time
        this.score = Math.floor((Date.now() - this.startTime) / 100);
        this.scoreDisplay.textContent = `Score: ${this.score}`;
        
        // Handle jump input
        if ((this.keys['ArrowUp'] || this.keys['w'] || this.keys['W']) && !this.llama.falling) {
            this.llama.y -= JUMP_SPEED;
            
            if (this.llama.y < MIN_Y) {
                this.llama.y = MIN_Y;
                this.llama.falling = true;
            }
        }
        
        // Apply gravity
        if (this.llama.y < LLAMA_Y) {
            this.llama.y += GRAVITY;
        } else {
            this.llama.y = LLAMA_Y;
            this.llama.falling = false;
        }
        
        // Move cactus with optimized calculation
        if (this.activeCactus.x >= 0) {
            // Gradually increase acceleration, but cap at max speed
            this.acceleration = Math.min(this.acceleration + 0.001, this.maxSpeed - this.speed);
            const currentSpeed = (this.speed + this.acceleration) * deltaTime;
            this.activeCactus.x -= currentSpeed;
        }
        
        // Reset cactus when it goes off screen
        if (this.activeCactus.x <= -CACTUS_WIDTH) {
            this.resetCactus();
        }
        
        // Collision detection
        this.checkCollision();
    }
    
    checkCollision() {
        // Optimized collision check - only check active cactus
        const cactus = this.activeCactus;
        if (this.llama.x < cactus.x + cactus.width &&
            this.llama.x + this.llama.width > cactus.x &&
            this.llama.y < cactus.y + cactus.height &&
            this.llama.y + this.llama.height > cactus.y) {
            
            // Collision detected
            this.currentState = GameState.GAME_OVER;
        }
    }
    
    render() {
        // Clear canvas
        this.ctx.clearRect(0, 0, GAME_WIDTH, GAME_HEIGHT);
        
        if (this.currentState === GameState.INIT) {
            this.renderInitScreen();
        } else if (this.currentState === GameState.PLAYING || this.currentState === GameState.PAUSED) {
            this.renderGame();
        } else if (this.currentState === GameState.GAME_OVER) {
            this.renderGameOver();
        }
    }
    
    renderInitScreen() {
        // Black background
        this.ctx.fillStyle = '#000000';
        this.ctx.fillRect(0, 0, GAME_WIDTH, GAME_HEIGHT);
        
        // Start screen text
        if (this.images.startScreen && this.images.startScreen.complete) {
            this.ctx.drawImage(this.images.startScreen, 250, 170, 640, 40);
        }
        
        // Logo
        if (this.images.logo && this.images.logo.complete) {
            this.ctx.drawImage(this.images.logo, 450, 520, 148, 249);
        }
        
        // Studio text
        if (this.images.studio && this.images.studio.complete) {
            this.ctx.drawImage(this.images.studio, 450, 480, 149, 30);
        }
    }
    
    renderGame() {
        // Sky blue background
        this.ctx.fillStyle = '#00b4ff';
        this.ctx.fillRect(0, 0, GAME_WIDTH, GAME_HEIGHT);
        
        // Sky image
        if (this.images.sky && this.images.sky.complete) {
            this.ctx.drawImage(this.images.sky, 0, 0, GAME_WIDTH, 441);
        }
        
        // Grass
        this.ctx.fillStyle = '#00ff00';
        this.ctx.fillRect(0, FLOOR_Y, GAME_WIDTH, GRASS_HEIGHT);
        
        // Floor
        this.ctx.fillStyle = '#b49b00';
        this.ctx.fillRect(0, FLOOR_Y, GAME_WIDTH, FLOOR_HEIGHT);
        
        // Llama
        if (this.images.llama && this.images.llama.complete) {
            this.ctx.drawImage(this.images.llama, this.llama.x, this.llama.y, this.llama.width, this.llama.height);
        }
        
        // Active cactus
        if (this.images.cactus && this.images.cactus.complete) {
            this.ctx.drawImage(this.images.cactus, this.activeCactus.x, this.activeCactus.y, CACTUS_WIDTH, CACTUS_HEIGHT);
        }
        
        // Pause indicator
        if (this.currentState === GameState.PAUSED) {
            if (this.images.pause && this.images.pause.complete) {
                this.ctx.drawImage(this.images.pause, 50, 20, 150, 30);
            } else {
                this.ctx.fillStyle = 'white';
                this.ctx.font = '30px Arial';
                this.ctx.fillText('PAUSED', 50, 50);
            }
        }
    }
    
    renderGameOver() {
        // Black background
        this.ctx.fillStyle = '#000000';
        this.ctx.fillRect(0, 0, GAME_WIDTH, GAME_HEIGHT);
        
        // Game Over text
        if (this.images.gameOver && this.images.gameOver.complete) {
            this.ctx.drawImage(this.images.gameOver, 230, 170, 640, 57);
        }
        
        // Game over llama
        if (this.images.restartLlama && this.images.restartLlama.complete) {
            this.ctx.drawImage(this.images.restartLlama, 400, 250, 217, 210);
        }
        
        // Restart text
        if (this.images.startScreen && this.images.startScreen.complete) {
            this.ctx.drawImage(this.images.startScreen, 230, 480, 640, 40);
        }
        
        // Display final score
        this.ctx.fillStyle = 'white';
        this.ctx.font = '36px Arial';
        this.ctx.fillText(`Final Score: ${this.score}`, 400, 380);
    }
    
    gameLoop(currentTime) {
        this.update(currentTime);
        this.render();
        requestAnimationFrame(this.gameLoop);
    }
}

// Start the game when page loads
window.addEventListener('load', () => {
    new LlamaGame();
});

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

const JUMP_SPEED = 28;
const GRAVITY = 2600; // px/s^2
const MIN_Y = 10;
const MOVE_SPEED = 350; // px/s
const SPAWN_MIN_TIME = 1.1;
const SPAWN_MAX_TIME = 2.3;
const GROUND_STRIPE_WIDTH = 80;

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
        this.speed = 220;
        this.acceleration = 0;
        this.maxSpeed = 520; // Cap maximum speed (pixels/sec)
        
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
        this.cacti = [];
        this.nextSpawnTime = 0;
        
        // Images
        this.images = {};
        this.imagesLoaded = 0;
        this.totalImages = 0;
        
        // Input
        this.keys = {};
        this.touchControls = {
            left: false,
            right: false,
            jump: false
        };
        
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
        this.cacti = [];
        this.scheduleNextSpawn();
    }

    scheduleNextSpawn() {
        const now = performance.now() / 1000;
        const interval = SPAWN_MIN_TIME + Math.random() * (SPAWN_MAX_TIME - SPAWN_MIN_TIME);
        this.nextSpawnTime = now + interval;
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
        
        // Touch controls setup
        this.setupTouchControls();
    }
    
    setupTouchControls() {
        // Create touch control elements
        const gameContainer = document.getElementById('game-container');
        
        // Create touch controls container
        const touchControls = document.createElement('div');
        touchControls.id = 'touch-controls';
        touchControls.innerHTML = `
            <button id="btn-left" class="touch-btn touch-btn-left">←</button>
            <button id="btn-right" class="touch-btn touch-btn-right">→</button>
            <button id="btn-jump" class="touch-btn touch-btn-jump">JUMP</button>
        `;
        gameContainer.appendChild(touchControls);
        
        // Get button references
        const btnLeft = document.getElementById('btn-left');
        const btnRight = document.getElementById('btn-right');
        const btnJump = document.getElementById('btn-jump');
        
        // Touch event handlers
        const handleTouchStart = (control) => (e) => {
            e.preventDefault();
            this.touchControls[control] = true;
            
            // Handle game state changes for jump button
            if (control === 'jump') {
                if (this.currentState === GameState.INIT) {
                    this.currentState = GameState.PLAYING;
                    this.resetGame();
                } else if (this.currentState === GameState.GAME_OVER) {
                    this.currentState = GameState.PLAYING;
                    this.resetGame();
                }
            }
        };
        
        const handleTouchEnd = (control) => (e) => {
            e.preventDefault();
            this.touchControls[control] = false;
        };
        
        // Add event listeners for each button
        ['touchstart', 'mousedown'].forEach(event => {
            btnLeft.addEventListener(event, handleTouchStart('left'));
            btnRight.addEventListener(event, handleTouchStart('right'));
            btnJump.addEventListener(event, handleTouchStart('jump'));
        });
        
        ['touchend', 'touchcancel', 'mouseup'].forEach(event => {
            btnLeft.addEventListener(event, handleTouchEnd('left'));
            btnRight.addEventListener(event, handleTouchEnd('right'));
            btnJump.addEventListener(event, handleTouchEnd('jump'));
        });
    }
    
    resetGame() {
        this.score = 0;
        this.startTime = Date.now();
        this.speed = 220;
        this.acceleration = 0;
        this.llama.x = LLAMA_X;
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
        const deltaTime = Math.min((currentTime - this.lastFrameTime) / 1000, 0.05); // seconds, cap for stability
        this.lastFrameTime = currentTime;
        
        // Update score based on time
        this.score = Math.floor((Date.now() - this.startTime) / 100);
        this.scoreDisplay.textContent = `Score: ${this.score}`;
        
        // Handle jump input
        if (((this.keys['ArrowUp'] || this.keys['w'] || this.keys['W']) || this.touchControls.jump) && !this.llama.falling) {
            this.llama.y -= JUMP_SPEED;
            
            if (this.llama.y < MIN_Y) {
                this.llama.y = MIN_Y;
                this.llama.falling = true;
            }
        }
        
        // Handle horizontal movement
        if ((this.keys['ArrowLeft'] || this.keys['a'] || this.keys['A']) || this.touchControls.left) {
            this.llama.x -= MOVE_SPEED * deltaTime;
            if (this.llama.x < 0) {
                this.llama.x = 0;
            }
        }
        if ((this.keys['ArrowRight'] || this.keys['d'] || this.keys['D']) || this.touchControls.right) {
            this.llama.x += MOVE_SPEED * deltaTime;
            if (this.llama.x > GAME_WIDTH - LLAMA_WIDTH) {
                this.llama.x = GAME_WIDTH - LLAMA_WIDTH;
            }
        }
        
        // Apply gravity
        if (this.llama.y < LLAMA_Y) {
            this.llama.y += GRAVITY * deltaTime;
        } else {
            this.llama.y = LLAMA_Y;
            this.llama.falling = false;
        }
        
        // Gradually increase acceleration, but cap at max speed
        this.acceleration = Math.min(this.acceleration + 40 * deltaTime, this.maxSpeed - this.speed);
        const currentSpeed = (this.speed + this.acceleration);

        // Spawn new cactus based on timer
        const now = currentTime / 1000;
        if (now >= this.nextSpawnTime) {
            this.cacti.push({
                x: GAME_WIDTH + Math.random() * 200,
                y: CACTUS_Y,
                width: CACTUS_WIDTH,
                height: CACTUS_HEIGHT
            });
            this.scheduleNextSpawn();
        }

        // Move cacti and remove off-screen ones
        this.cacti.forEach(c => {
            c.x -= currentSpeed * deltaTime;
        });
        this.cacti = this.cacti.filter(c => c.x > -CACTUS_WIDTH);

        // Collision detection
        this.checkCollision();
    }
    
    checkCollision() {
        for (const cactus of this.cacti) {
            if (this.llama.x < cactus.x + cactus.width &&
                this.llama.x + this.llama.width > cactus.x &&
                this.llama.y < cactus.y + cactus.height &&
                this.llama.y + this.llama.height > cactus.y) {
                this.currentState = GameState.GAME_OVER;
                break;
            }
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
        
        // Floor with moving stripes for forward perception
        this.ctx.fillStyle = '#b49b00';
        this.ctx.fillRect(0, FLOOR_Y, GAME_WIDTH, FLOOR_HEIGHT);
        this.ctx.fillStyle = '#d8bb37';
        const stripeOffset = (performance.now() / 8) % GROUND_STRIPE_WIDTH;
        for (let x = -GROUND_STRIPE_WIDTH; x < GAME_WIDTH; x += GROUND_STRIPE_WIDTH * 2) {
            this.ctx.fillRect(x + stripeOffset, FLOOR_Y, GROUND_STRIPE_WIDTH, FLOOR_HEIGHT);
        }
        
        // Grass
        this.ctx.fillStyle = '#00ff00';
        this.ctx.fillRect(0, FLOOR_Y, GAME_WIDTH, GRASS_HEIGHT);
        
        // Llama
        if (this.images.llama && this.images.llama.complete) {
            this.ctx.drawImage(this.images.llama, this.llama.x, this.llama.y, this.llama.width, this.llama.height);
        }
        
        // Cacti
        if (this.images.cactus && this.images.cactus.complete) {
            this.cacti.forEach(cactus => {
                this.ctx.drawImage(this.images.cactus, cactus.x, cactus.y, CACTUS_WIDTH, CACTUS_HEIGHT);
            });
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

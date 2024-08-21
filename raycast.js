const TILE_SIZE = 32;
const MAP_NUM_ROWS = 11;
const MAP_NUM_COLS = 15;

const WINDOW_WIDTH = MAP_NUM_COLS * TILE_SIZE;
const WINDOW_HEIGHT = MAP_NUM_ROWS * TILE_SIZE;

const FOV_ANGLE = 60 * Math.PI / 180 ;

const WALL_STRIP_WIDTH = 50;

const NUM_RAYS = WINDOW_WIDTH / WALL_STRIP_WIDTH;

class Map
{
    constructor()
    {
        this.grid = 
        [
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1],
            [1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1],
            [1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1],
            [1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1],
            [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
        ];
    }

    hasWallAt(x, y)
    {
        if(x<0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT)
        {
            return true;
        }
        var mapX = Math.floor(x/ TILE_SIZE);
        var mapY = Math.floor(y/ TILE_SIZE);
        return this.grid[mapY][mapX] != 0; 
    }

    render()
    {
        for( var i = 0; i <MAP_NUM_ROWS; i++)
            for( var j = 0; j < MAP_NUM_COLS ; j++)
            {
                var tileX = j * TILE_SIZE;
                var tileY = i * TILE_SIZE;
                var tileColor = this.grid[i][j] == 1 ? "#222" : "#fff";
                stroke("#222");
                fill(tileColor);
                rect(tileX, tileY, TILE_SIZE, TILE_SIZE);
            }
    }
}

class Player
{
    constructor()
    {
        this.x = WINDOW_WIDTH/2;
        this.y = WINDOW_HEIGHT/2;
        this.radius = 3;
        this.turnDirection = 0; // -1 if laft, 1 if right
        this.walkDirection = 0; // -1 if back, 1 if front  
        this.rotationAngle =  Math.PI / 2;
        this.moveSpeed = 2.0;
        this.rotationSpeed = 3 * Math.PI / 180 ;
    }

    update()
    {
        this.rotationAngle += this.turnDirection * this.rotationSpeed;
        
        var moveStep = this.walkDirection * this.moveSpeed;

        var newX = this.x + Math.cos(this.rotationAngle)* moveStep;
        var newY = this.y + Math.sin(this.rotationAngle)* moveStep;
        
        if(!grid.hasWallAt(newX, newY))
        {
            this.x = newX;
            this.y = newY;
        }
        else if (!grid.hasWallAt(this.x, newY))
        {
            this.y += Math.sin(this.rotationAngle)* moveStep / 2;
        }
        else if (!grid.hasWallAt(newX, this.y))
        {
            this.x += Math.cos(this.rotationAngle)* moveStep / 2;
        }
    }

    render()
    {
        noStroke();
        fill("red");
        circle(this.x, this.y, this.radius);
        stroke("red");
        line(
            this.x, 
            this.y,
            this.x + Math.cos(this.rotationAngle)*20,
            this.y + Math.sin(this.rotationAngle)*20
        )
    }
}

class Ray
{
    constructor(rayAngle)
    {
        this.rayAngle = normalizeAngle(rayAngle);
        this.wallHitX = 0;
        this.wallHitY = 0;
        this.distance = 0;

        this.isRayFacingDown = this.rayAngle > 0 && this.rayAngle < Math.PI;
        this.isRayFacingUp = !this.isRayFacingDown;

        this.isRayFacingRight = this.rayAngle < 0.5 * Math.PI || this.rayAngle > 1.5 * Math.PI;
        this.isRayFacingLeft = !this.isRayFacingRight;
    }

    cast(columeID)
    {
        var xintercept, yintercept;
        var xstep, ystep;

        ////////////////////////////////////////////
        // Horizontal Ray Grid Intersection
        ////////////////////////////////////////////

        var foundHorzWallHit = false;
        var wallHitX = 0;
        var wallHitY = 0;

        // Find the y-coordinate of the closest horizontal grid intersection
        yintercept = Math.floor(player.y / TILE_SIZE) * TILE_SIZE;
        yintercept += this.isRayFacingDown ? TILE_SIZE : 0;
        // Find the x-coordinate of the closest horizontal grid intersection
        xintercept = player.x + (yintercept - player.y) / Math.tan(this.rayAngle);

        // Calculate the increment xstep and ystep
        ystep = TILE_SIZE;
        ystep *= this.isRayFacingUp ? -1 : 1;

        xstep = TILE_SIZE / Math.tan(this.rayAngle);
        xstep *= (this.isRayFacingLeft && xstep > 0) ? -1 : 1;
        xstep *= (this.isRayFacingRight && xstep < 0) ? -1 : 1;

        var nextHorzTouchX = xintercept;
        var nextHorzTouchY = yintercept;

        if(this.isRayFacingUp)
            nextHorzTouchY--;

        // Increment xstep and ystep until we find a wall
        while(nextHorzTouchX >= 0 && nextHorzTouchX <= WINDOW_WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= WINDOW_HEIGHT)
        {
            if(grid.hasWallAt(nextHorzTouchX, nextHorzTouchY))
            {
                // We found a wall
                foundHorzWallHit = true;
                this.wallHitX = nextHorzTouchX;
                this.wallHitY = nextHorzTouchY;

                stroke ("red");
                line(player.x, player.y, this.wallHitX, this.wallHitY);

                break;
            }
            else
            {
                nextHorzTouchX += xstep;
                nextHorzTouchY += ystep;
            }
        }

    }

    render()
    {
        //stroke("rgba(255, 0, 0, 100)");
        //line(
        //    player.x, 
        //    player.y, 
        //    player.x + Math.cos(this.rayAngle) * 30,
        //    player.y + Math.sin(this.rayAngle) * 30
        //);
    }
}

var grid = new Map();
var player = new Player();
var rays = [];

function CheckIfKeydown()
{
    // Update x and y if an arrow key is pressed.
    if(keyIsDown(LEFT_ARROW) == true && keyIsDown(RIGHT_ARROW) == true)
    {
        player.turnDirection = 0;
    }
    else
    {
        if (keyIsDown(LEFT_ARROW) == true) {
            player.turnDirection = -1;
        }
    
        if (keyIsDown(RIGHT_ARROW) == true) {
            player.turnDirection = +1;
        }
    }

    if(keyIsDown(UP_ARROW) == true && keyIsDown(DOWN_ARROW) == true)
    {
        player.walkDirection = 0;
    }
    else
    {
        if (keyIsDown(UP_ARROW) == true) {
            player.walkDirection = +1;
        }
    
        if (keyIsDown(DOWN_ARROW) == true) {
            player.walkDirection = -1;
        }
    }
}

function keyReleased()
{
    if( keyCode == UP_ARROW)
    {
        player.walkDirection = 0;
    } else if ( keyCode == DOWN_ARROW)
    {
        player.walkDirection = 0;
    }
    if ( keyCode == RIGHT_ARROW)
    {
        player.turnDirection = 0;
    } 
    if ( keyCode == LEFT_ARROW)
    {
        player.turnDirection = 0;
    }

}

// normalize the angle to be between 0 and 2*PI
function normalizeAngle(angle)
{
    angle = angle % (2 * Math.PI);
    if(angle < 0)
    {
        angle = (2 * Math.PI) + angle;
    }
    return angle;
}

function setup()
{
    createCanvas(WINDOW_WIDTH, WINDOW_HEIGHT);
}

function castAllRays()
{
    var columnId = 0;

    var rayAngle = player.rotationAngle - (FOV_ANGLE / 2);

    rays = [];

    for( var i =0; i<NUM_RAYS; i++)
    {
        var ray = new Ray(rayAngle);
        ray.cast(columnId)
        rays.push(ray);

        rayAngle += FOV_ANGLE / NUM_RAYS;

        columnId++;
    }   
}

function update()
{
    CheckIfKeydown();
    player.update();
    //castAllRays();
}

function draw()
{
    update();

    grid.render();
    player.render();

    for(ray of rays)
    {
        ray.render();
    }

    castAllRays();
}

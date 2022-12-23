
const canvas = document.getElementById("myCanvas");
const context = canvas.getContext("2d");

var is_start = false;
var is_menu = true;
var is_game_finished = false;
var i = 1;

var draw_menu = function () {
    var bac = new Image();
    bac.src = "menu-background.jpg";
    bac.onload = function () {
        context.drawImage(bac, 300, 50);
    };
    var image1 = new Image();
    image1.src = "start.png";
    image1.onload = function () {
        context.drawImage(image1, 530, 210, 200, 120);
    };
    var image3 = new Image();
    image3.src = "help.png";
    image3.onload = function () {
        context.drawImage(image3, 530, 150, 200, 120);
    };


    var image4 = new Image();
    image4.src = "game-title.png";
    image4.onload = function () {
        context.drawImage(image4, 480, 30, 300, 240);
    };
};


canvas.addEventListener('mousedown', function (e) {
    var mX = e.clientX - canvas.offsetLeft;
    var mY = e.clientY - canvas.offsetTop;

    if (is_start) {
        Characters.prototype.getUp(true);
    }

    if (is_menu && mX >= 570 && mX <= 690 && mY >= 250 && mY <= 280) {
        var image2 = new Image();
        image2.src = "start-mousedown.png";
        image2.onload = function () {
            context.drawImage(image2, 530, 210, 200, 120);

        }
    }
    if (is_menu && mX >= 570 && mX <= 690 && mY >= 190 && mY <= 220) {
        var image2 = new Image();
        image2.src = "help-mousedown.png";
        image2.onload = function () {
            context.drawImage(image2, 530, 150, 200, 120);
        }
    }

    if(is_game_finished && mX >= 570 && mX <= 690 && mY >= 210 && mY <= 230){
        is_menu=true;
        is_game_finished=false;
        i=1;
        context.clearRect(0,0,canvas.width,canvas.height);
        draw_menu();
    }
}
);

canvas.addEventListener('mouseup', function (e) {
    var mX = e.clientX - canvas.offsetLeft;
    var mY = e.clientY - canvas.offsetTop;
    if (is_menu && mX >= 570 && mX <= 690 && mY >= 250 && mY <= 280) {
        is_menu = false;
        context.clearRect(0,0,canvas.width,canvas.height);
        start();
    }

    if (is_menu && mX >= 570 && mX <= 690 && mY >= 190 && mY <= 220) {
        var helpScreen = new Image();
        helpScreen.src = "helpscreen.png";
        helpScreen.onload = function () {
            context.drawImage(helpScreen, 430, 30, 400, 400);
        }
    }
    if (is_menu && mX >= 760 && mX <= 780 && mY >= 280 && mY <= 300) {
        draw_menu();
    }
    if (is_game_over && mX >= 555 && mX <= 725 && mY >= 205 && mY <= 230) {
        i = 1;
        is_game_over = false;
        is_start = true;
        next = true;
        draw_menu();
    }
});


var Level = function (i, image) {
    this.i = i;
    this.image = image;
};


Level.prototype.draw = function (ctx) {
    if (this.i == 1) {
        ctx.drawImage(this.image, 500, 310, 75, 100);
    }
    if (this.i == 2) {
        ctx.drawImage(this.image, 270, 345, 120, 70);
        ctx.drawImage(this.image, 720, 325, 120, 90);
    }
    if (this.i == 3) {
        ctx.drawImage(this.image, 0, 225, 96, 50);
        ctx.drawImage(this.image, 96, 225, 96, 50);
        ctx.drawImage(this.image, 192, 225, 96, 50);
        ctx.drawImage(this.image, 288, 225, 96, 50);
        ctx.drawImage(this.image, 384, 225, 96, 50);
        ctx.drawImage(this.image, 480, 225, 96, 50);
        ctx.drawImage(this.image, 700, 305, 90, 120);
    }
    if (this.i == 4) {
        ctx.drawImage(this.image, 450, 225, 90, 50);
        ctx.drawImage(this.image, 540, 225, 90, 50);
        ctx.drawImage(this.image, 630, 225, 90, 50);
        ctx.drawImage(this.image, 720, 225, 90, 50);
        ctx.drawImage(this.image, 900, 305, 80, 120);
        ctx.drawImage(this.image, 250, 325, 80, 95);
    }
    if (this.i == 5) {
        ctx.drawImage(this.image, 270, 275, 80, 140);
        ctx.drawImage(this.image, 600, 335, 140, 80);
        ctx.drawImage(this.image, 915, 235, 100, 50);
        ctx.drawImage(this.image, 1015, 235, 100, 50);
        ctx.drawImage(this.image, 1115, 235, 100, 50);
        ctx.drawImage(this.image, 1215, 235, 85, 50);
    }
};


var Characters = function (image, x, y) {
    this.image = image;
    this.x = x;
    this.y = y;
    this.width = image.width;
    this.height = image.height / 8;
    this.frm = 0;
    this.dis = 0;
    this.upfrm = 0;
    this.updis = 0;
};

Characters.prototype.getUp = function (bool) {
    this.isUp = bool;
};

Characters.prototype.draw = function (ctx) {

    if (!this.isUp) {
        ctx.save();
        ctx.translate(this.x, this.y);
        ctx.drawImage(this.image, 0, this.frm * this.height, this.width, this.height, 0, 0, this.width, this.height);
        ctx.restore();
        this.x += 5;
        this.dis++;
        if (this.dis >= 10) {
            this.dis = 0;
            this.frm++;
            if (this.frm >= 8) this.frm = 0;
        }
    }
    if (this.isUp) {
        ctx.save();
        ctx.translate(this.x, this.y);
        ctx.drawImage(this.image, 0, this.upfrm * this.height, this.width, this.height, 0, 0, this.width, this.height);
        ctx.restore();
        this.x += 5;
        var vel = 14 - this.upfrm * 4;
        if (this.upfrm < 4) this.y -= vel;
        if (this.upfrm >= 4) this.y -= vel;
        this.updis++;
        if (this.updis >= 7) {
            this.updis = 0;
            this.upfrm++;
            if (this.upfrm >= 8) {
                this.upfrm = 0;
                Characters.prototype.getUp(false);
            }
        }
    }
};

Characters.prototype.getCrash = function () {

    if (i == 1) {
        var inboxX = (this.x >= 520 - this.width) && (this.x <= 555);
        var inboxY = (this.y >= 320 - this.height);
        var inbox = (inboxX) && (inboxY);
        return inbox;
    }
    if (i == 2) {
        var inboxX1 = (this.x >= 290 - this.width) && (this.x <= 370);
        var inboxY1 = (this.y >= 355 - this.height);
        var inboxX2 = (this.x >= 740 - this.width) && (this.x <= 820);
        var inboxY2 = (this.y >= 335 - this.height);
        var inbox1 = (inboxX1) && (inboxY1);
        var inbox2 = (inboxX2) && (inboxY2);
        var inbox = inbox1 || inbox2;
        return inbox;
    }
    if (i == 3) {
        var inboxX1 = (this.x >= 0 - this.width) && (this.x <= 556);
        var inboxY1 = (this.y >= 235 - this.height) && (this.y <= 245);
        var inboxX2 = (this.x >= 720 - this.width) && (this.x <= 770);
        var inboxY2 = (this.y >= 315 - this.height);
        var inbox1 = (inboxX1) && (inboxY1);
        var inbox2 = (inboxX2) && (inboxY2);
        var inbox = inbox1 || inbox2;
        return inbox;
    }
    if (i == 4) {
        var inboxX1 = (this.x >= 280 - this.width) && (this.x <= 310);
        var inboxY1 = (this.y >= 335 - this.height);
        var inboxX2 = (this.x >= 930 - this.width) && (this.x <= 960);
        var inboxY2 = (this.y >= 315 - this.height);
        var inboxX3 = (this.x >= 480 - this.width) && (this.x <= 760);
        var inboxY3 = (this.y >= 235 - this.height) && (this.y <= 245);
        var inbox1 = (inboxX1) && (inboxY1);
        var inbox2 = (inboxX2) && (inboxY2);
        var inbox3 = (inboxX3) && (inboxY3);
        var inbox = inbox1 || inbox2 || inbox3;
        return inbox;
    }
    if (i == 5) {
        var inboxX1 = (this.x >= 300 - this.width) && (this.x <= 330);
        var inboxY1 = (this.y >= 295 - this.height);
        var inboxX2 = (this.x >= 635 - this.width) && (this.x <= 710);
        var inboxY2 = (this.y >= 355 - this.height);
        var inboxX3 = (this.x >= 935 - this.width) && (this.x <= 1300);
        var inboxY3 = (this.y >= 245 - this.height) && (this.y <= 275);
        var inbox1 = (inboxX1) && (inboxY1);
        var inbox2 = (inboxX2) && (inboxY2);
        var inbox3 = (inboxX3) && (inboxY3);
        var inbox = inbox1 || inbox2 || inbox3;
        return inbox;
    }
};

var start = function () {
    is_start = true;
    next = true;
    var background = new Image();
    var characterImg = new Image();
    var obstacleImage = new Image();
    var game_finished = new Image();
    game_finished.src = "game-over.png";
    obstacleImage.src = "obstacle.png";
    background.src = "play-background.jpg";
    characterImg.src = "character-actions.png";

    characterImg.onload = function () {
        var character = new Characters(characterImg, 0, 295);
        interval=setInterval(function () {
            var level = new Level(i, obstacleImage);
            context.clearRect(0, 0, 1300, 450);
            context.drawImage(background, 0, 0, 1300, 450);
            context.fillStyle = "black";
            context.fillRect(0, 410, 1300, 40);
            level.draw(context);
            character.draw(context);
            if (character.getCrash(i)) {
                character.x = 0;
            }
            if (character.x >= 1300 && next === true) {
                character.x = 0;
                i++;
                if (i >= 5) {
                    next = false;
                }
            }
            if (i >= 5 && next === false) {
                context.drawImage(game_finished, 275, 25, 750, 350);
                is_game_finished = true;
                is_start = false;
                clearInterval(interval);
            }
        }, 20);

    };
};

draw_menu();
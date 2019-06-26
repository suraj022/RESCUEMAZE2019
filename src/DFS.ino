void setWalls() {
  switch (HEAD) {
    //////////////////////////////////////////////////////////////////////
  case 0: // Heading north
    if (getDistance(FRONT) < 8000)
      if (getDistance(FRONT) < WALLDISTANCE)
        maze[mazeNum].cell[maze[mazeNum].COUNT].N = true;
      else
        maze[mazeNum].cell[maze[mazeNum].COUNT].N = false;
    else
      maze[mazeNum].cell[maze[mazeNum].COUNT].N = true;

    if (getDistance(RIGHT) < 8000)
      if (getDistance(RIGHT) < WALLDISTANCE)
        maze[mazeNum].cell[maze[mazeNum].COUNT].E = true;
      else
        maze[mazeNum].cell[maze[mazeNum].COUNT].E = false;
    else
      maze[mazeNum].cell[maze[mazeNum].COUNT].E = true;

    if (getDistance(LEFT) < 8000)
      if (getDistance(LEFT) < WALLDISTANCE)
        maze[mazeNum].cell[maze[mazeNum].COUNT].W = true;
      else
        maze[mazeNum].cell[maze[mazeNum].COUNT].W = false;
    else
      maze[mazeNum].cell[maze[mazeNum].COUNT].W = true;

    if (maze[mazeNum].COUNT == 1)
      maze[mazeNum].cell[maze[mazeNum].COUNT].S = true;
    break;
    //////////////////////////////////////////////////////////////////////
  case 1: // heading east
    if (getDistance(LEFT) < 8000)
      if (getDistance(LEFT) < WALLDISTANCE)
        maze[mazeNum].cell[maze[mazeNum].COUNT].N = true;
      else
        maze[mazeNum].cell[maze[mazeNum].COUNT].N = false;
    else
      maze[mazeNum].cell[maze[mazeNum].COUNT].N = true;

    if (getDistance(FRONT) < 8000)
      if (getDistance(FRONT) < WALLDISTANCE)
        maze[mazeNum].cell[maze[mazeNum].COUNT].E = true;
      else
        maze[mazeNum].cell[maze[mazeNum].COUNT].E = false;
    else
      maze[mazeNum].cell[maze[mazeNum].COUNT].E = true;

    if (getDistance(RIGHT) < 8000)
      if (getDistance(RIGHT) < WALLDISTANCE)
        maze[mazeNum].cell[maze[mazeNum].COUNT].S = true;
      else
        maze[mazeNum].cell[maze[mazeNum].COUNT].S = false;
    else
      maze[mazeNum].cell[maze[mazeNum].COUNT].S = true;

    if (maze[mazeNum].COUNT == 1)
      maze[mazeNum].cell[maze[mazeNum].COUNT].W = true;
    break;
    //////////////////////////////////////////////////////////////////////
  case 2: // heading south
    if (getDistance(LEFT) < 8000)
      if (getDistance(LEFT) < WALLDISTANCE)
        maze[mazeNum].cell[maze[mazeNum].COUNT].E = true;
      else
        maze[mazeNum].cell[maze[mazeNum].COUNT].E = false;
    else
      maze[mazeNum].cell[maze[mazeNum].COUNT].E = true;

    if (getDistance(FRONT) < 8000)
      if (getDistance(FRONT) < WALLDISTANCE)
        maze[mazeNum].cell[maze[mazeNum].COUNT].W = true;
      else
        maze[mazeNum].cell[maze[mazeNum].COUNT].W = false;
    else
      maze[mazeNum].cell[maze[mazeNum].COUNT].W = true;

    if (getDistance(RIGHT) < 8000)
      if (getDistance(RIGHT) < WALLDISTANCE)
        maze[mazeNum].cell[maze[mazeNum].COUNT].S = true;
      else
        maze[mazeNum].cell[maze[mazeNum].COUNT].S = false;
    else
      maze[mazeNum].cell[maze[mazeNum].COUNT].S = true;

    if (maze[mazeNum].COUNT == 1)
      maze[mazeNum].cell[maze[mazeNum].COUNT].N = true;
    break;
    //////////////////////////////////////////////////////////////////////
  case 3: // heading west
    if (getDistance(RIGHT) < 8000)
      if (getDistance(RIGHT) < WALLDISTANCE)
        maze[mazeNum].cell[maze[mazeNum].COUNT].N = true;
      else
        maze[mazeNum].cell[maze[mazeNum].COUNT].N = false;
    else
      maze[mazeNum].cell[maze[mazeNum].COUNT].N = true;

    if (getDistance(LEFT) < 8000)
      if (getDistance(LEFT) < WALLDISTANCE)
        maze[mazeNum].cell[maze[mazeNum].COUNT].S = true;
      else
        maze[mazeNum].cell[maze[mazeNum].COUNT].S = false;
    else
      maze[mazeNum].cell[maze[mazeNum].COUNT].S = true;

    if (getDistance(FRONT) < 8000)
      if (getDistance(FRONT) < WALLDISTANCE)
        maze[mazeNum].cell[maze[mazeNum].COUNT].W = true;
      else
        maze[mazeNum].cell[maze[mazeNum].COUNT].W = false;
    else
      maze[mazeNum].cell[maze[mazeNum].COUNT].W = true;

    if (maze[mazeNum].COUNT == 1)
      maze[mazeNum].cell[maze[mazeNum].COUNT].E = true;
    break;
  }
  //////////////////////////////////////////////////////////////////////
  int ways = 0;
  bitWrite(ways, 2, (getDistance(RIGHT) < WALLDISTANCE));
  bitWrite(ways, 1, (getDistance(FRONT) < WALLDISTANCE));
  bitWrite(ways, 0, (getDistance(LEFT) < WALLDISTANCE));

  maze[mazeNum].cell[maze[mazeNum].COUNT].node =
      ((ways >= 0 && ways <= 2) || ways == 4);
}

bool nextTile(int x, int y) {
  int i = maze[mazeNum].COUNT - 1;
  switch (HEAD) {
  case 0: // north
    while (i > 0) {
      if (maze[mazeNum].cell[i].x == x && maze[mazeNum].cell[i].y == y + 1) {
        maze[mazeNum].cell[i].IS = true;
        return true;
      }
      i--;
    }
    break;
  case 1: // east
    while (i > 0) {
      if (maze[mazeNum].cell[i].x == x + 1 && maze[mazeNum].cell[i].y == y) {
        maze[mazeNum].cell[i].IW = true;
        return true;
      }
      i--;
    }
    break;
  case 2: // south
    while (i > 0) {
      if (maze[mazeNum].cell[i].x == x && maze[mazeNum].cell[i].y == y - 1) {
        maze[mazeNum].cell[i].IN = true;
        return true;
      }
      i--;
    }
    break;
  case 3: // west
    while (i > 0) {
      if (maze[mazeNum].cell[i].x == x - 1 && maze[mazeNum].cell[i].y == y) {
        maze[mazeNum].cell[i].IE = true;
        return true;
      }
      i--;
    }
    break;
  }
  return false;
}

bool setHeading() {
  int head = -1;
  bool flag = false;
  if (!maze[mazeNum].cell[maze[mazeNum].COUNT].S &&
      maze[mazeNum].cell[maze[mazeNum].COUNT].testCount < 1 &&
      !maze[mazeNum].cell[maze[mazeNum].COUNT].IS && flag == false) {
    if (prevTile(2, maze[mazeNum].cell[maze[mazeNum].COUNT].x,
                 maze[mazeNum].cell[maze[mazeNum].COUNT].y)) {
      maze[mazeNum].cell[maze[mazeNum].COUNT].backWay = 2;
    } else {
      head = 2;
      flag = true;
    }
    maze[mazeNum].cell[maze[mazeNum].COUNT].testCount = 1;
  }
  if (!maze[mazeNum].cell[maze[mazeNum].COUNT].W &&
      maze[mazeNum].cell[maze[mazeNum].COUNT].testCount < 2 &&
      !maze[mazeNum].cell[maze[mazeNum].COUNT].IW && flag == false) {
    if (prevTile(3, maze[mazeNum].cell[maze[mazeNum].COUNT].x,
                 maze[mazeNum].cell[maze[mazeNum].COUNT].y)) {
      maze[mazeNum].cell[maze[mazeNum].COUNT].backWay = 3;
    } else {
      head = 3;
      flag = true;
    }
    maze[mazeNum].cell[maze[mazeNum].COUNT].testCount = 2;
  }
  if (!maze[mazeNum].cell[maze[mazeNum].COUNT].N &&
      maze[mazeNum].cell[maze[mazeNum].COUNT].testCount < 3 &&
      !maze[mazeNum].cell[maze[mazeNum].COUNT].IN && flag == false) {
    if (prevTile(0, maze[mazeNum].cell[maze[mazeNum].COUNT].x,
                 maze[mazeNum].cell[maze[mazeNum].COUNT].y)) {
      maze[mazeNum].cell[maze[mazeNum].COUNT].backWay = 0;
    } else {
      head = 0;
      flag = true;
    }
    maze[mazeNum].cell[maze[mazeNum].COUNT].testCount = 3;
  }
  if (!maze[mazeNum].cell[maze[mazeNum].COUNT].E &&
      maze[mazeNum].cell[maze[mazeNum].COUNT].testCount < 4 &&
      !maze[mazeNum].cell[maze[mazeNum].COUNT].IE && flag == false) {
    if (prevTile(1, maze[mazeNum].cell[maze[mazeNum].COUNT].x,
                 maze[mazeNum].cell[maze[mazeNum].COUNT].y)) {
      maze[mazeNum].cell[maze[mazeNum].COUNT].backWay = 1;
    } else {
      head = 1;
      flag = true;
    }
    maze[mazeNum].cell[maze[mazeNum].COUNT].testCount = 4;
  }

  if (!flag) {
    return false;
  } else {
    orient(head);
    return true;
  }
}

void orient(int head) {
  int result = head - HEAD;
  switch (result) {
  case -3:
  case 1:
    turnBot(90, 1, true);
    break;
  case -2:
  case 2:
    if (getDistance(LEFT) > getDistance(RIGHT)) {
      turnBot(90, -1, true);
      turnBot(90, -1, true);
    } else {
      turnBot(90, 1, true);
      turnBot(90, 1, true);
    }
    moveMotor(-50, -50);
    delay(200);
    moveMotor(0, 0);
    break;
  case -1:
  case 3:
    turnBot(90, -1, true);
    break;
  case 0:
    break;
  }
  indicateWalls();
  delay(20);
}

bool prevTile(int head, int x, int y) {
  bool flag = false;
  switch (head) {
  case 0: // north
    if (maze[mazeNum].cell[maze[mazeNum].COUNT - 1].x == x &&
        maze[mazeNum].cell[maze[mazeNum].COUNT - 1].y == y + 1)
      flag = true;
    break;
  case 1: // east
    if (maze[mazeNum].cell[maze[mazeNum].COUNT - 1].x == x + 1 &&
        maze[mazeNum].cell[maze[mazeNum].COUNT - 1].y == y)
      flag = true;
    break;
  case 2: // south
    if (maze[mazeNum].cell[maze[mazeNum].COUNT - 1].x == x &&
        maze[mazeNum].cell[maze[mazeNum].COUNT - 1].y == y - 1)
      flag = true;
    break;
  case 3: // west
    if (maze[mazeNum].cell[maze[mazeNum].COUNT - 1].x == x - 1 &&
        maze[mazeNum].cell[maze[mazeNum].COUNT - 1].y == y)
      flag = true;
    break;
  }
  return flag;
}

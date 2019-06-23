void setWalls() {
  switch (HEAD) {
  case 0: // Heading north
    cell[COUNT].N = (getDistance(FRONT) < WALLDISTANCE);
    cell[COUNT].E = (getDistance(RIGHT) < WALLDISTANCE);
    cell[COUNT].S = false;
    cell[COUNT].W = (getDistance(LEFT) < WALLDISTANCE);
    break;
  case 1: // heading east
    cell[COUNT].N = (getDistance(LEFT) < WALLDISTANCE);
    cell[COUNT].E = (getDistance(FRONT) < WALLDISTANCE);
    cell[COUNT].S = (getDistance(RIGHT) < WALLDISTANCE);
    cell[COUNT].W = false;
    break;
  case 2: // heading south
    cell[COUNT].N = false;
    cell[COUNT].E = (getDistance(LEFT) < WALLDISTANCE);
    cell[COUNT].S = (getDistance(FRONT) < WALLDISTANCE);
    cell[COUNT].W = (getDistance(RIGHT) < WALLDISTANCE);
    break;
  case 3: // heading west
    cell[COUNT].N = (getDistance(RIGHT) < WALLDISTANCE);
    if (COUNT != 0)
      cell[COUNT].E = false;
    cell[COUNT].S = (getDistance(LEFT) < WALLDISTANCE);
    cell[COUNT].W = (getDistance(FRONT) < WALLDISTANCE);
    break;
  }
  int ways = 0;
  bitWrite(ways, 2, (getDistance(RIGHT) < WALLDISTANCE));
  bitWrite(ways, 1, (getDistance(FRONT) < WALLDISTANCE));
  bitWrite(ways, 0, (getDistance(LEFT) < WALLDISTANCE));

  cell[COUNT].node = ((ways >= 0 && ways <= 2) || ways == 4);
}

bool nextTile(int x, int y) {
  bool flag = false;
  int i = COUNT;
  switch (HEAD) {
  case 0: // north
    while (i >= 0) {
      if (cell[i].x == x && cell[i].y == y + 1)
        flag = true;
      i--;
    }
    break;
  case 1: // east
    while (i >= 0) {
      if (cell[i].x == x + 1 && cell[i].y == y)
        flag = true;
      i--;
    }
    break;
  case 2: // south
    while (i >= 0) {
      if (cell[i].x == x && cell[i].y == y - 1)
        flag = true;
      i--;
    }
    break;
  case 3: // west
    while (i >= 0) {
      if (cell[i].x == x - 1 && cell[i].y == y)
        flag = true;
      i--;
    }
    break;
  }
  return flag;
}

bool setHeading() {
  int head = -1;
  bool flag = false;
  if (!cell[COUNT].S && cell[COUNT].testCount < 1 && flag == false) {
    if (prevTile(2, cell[COUNT].x, cell[COUNT].y)) {
      cell[COUNT].backWay = 2;
    } else {
      head = 2;
      flag = true;
    }
    cell[COUNT].testCount = 1;
  }
  if (!cell[COUNT].W && cell[COUNT].testCount < 2 && flag == false) {
    if (prevTile(3, cell[COUNT].x, cell[COUNT].y)) {
      cell[COUNT].backWay = 3;
    } else {
      head = 3;
      flag = true;
    }
    cell[COUNT].testCount = 2;
  }
  if (!cell[COUNT].N && cell[COUNT].testCount < 3 && flag == false) {
    if (prevTile(0, cell[COUNT].x, cell[COUNT].y)) {
      cell[COUNT].backWay = 0;
    } else {
      head = 0;
      flag = true;
    }
    cell[COUNT].testCount = 3;
  }

  if (!cell[COUNT].E && cell[COUNT].testCount < 4 && flag == false) {
    if (prevTile(1, cell[COUNT].x, cell[COUNT].y)) {
      cell[COUNT].backWay = 1;
    } else {
      head = 1;
      flag = true;
    }
    cell[COUNT].testCount = 4;
  }

  if (cell[COUNT].testCount > 3) {
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
    turn90(90, 1, true);
    break;
  case -2:
  case 2:
    if (getDistance(LEFT) > getDistance(RIGHT)) {
      turn90(90, -1, false);
      turn90(90, -1, true);
    } else {
      turn90(90, 1, false);
      turn90(90, 1, true);
    }
    break;
  case -1:
  case 3:
    turn90(90, -1, true);
    break;
  case 0:
    break;
  }
  indicateWalls();
  delay(200);
}

bool prevTile(int head, int x, int y) {
  bool flag = false;
  switch (head) {
  case 0: // north
    if (cell[COUNT - 1].x == x && cell[COUNT - 1].y == y - 1)
      flag = true;
    break;
  case 1: // east
    if (cell[COUNT - 1].x == x - 1 && cell[COUNT - 1].y == y)
      flag = true;
    break;
  case 2: // south
    if (cell[COUNT - 1].x == x && cell[COUNT - 1].y == y + 1)
      flag = true;
    break;
  case 3: // west
    if (cell[COUNT - 1].x == x + 1 && cell[COUNT - 1].y == y)
      flag = true;
    break;
  }
  return flag;
}

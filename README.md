**Introduction:**

This project is a computer vision system for analyzing video footage of "Eight Ball" billiard matches. The system processes video clips to identify and track billiard balls on the table, distinguishing between the cue ball, the black "8-ball", solid colored balls, and striped balls. The results are displayed as a 2D top-view minimap that updates with each frame to show the positions and trajectories of the balls.

**ToDo:**

1.Recognizes and localizes all balls on the playing field.

2.Distinguishes balls based on their category (cue ball, 8-ball, solid colors, stripes).

3.Detects the boundaries of the playing field.

4.Segments the playing field and the balls.

5.Provides a 2D top-view visualization of the current game state.

6.Tracks the trajectories of the balls after a shot.

**How to run**
```
mkdir build
cd build
cmake ..
make
```
```
./main ../dataset/game1_clip1.mp4
```

# Project 1: Brush

Please fill this out for Brush only. The project handout can be found [here](https://cs1230.graphics/projects/raster/1).

## Output Comparison
This is how you will demonstrate the functionality of your project.

For each of the rows below, follow the instructions to record a video. Please reference the [Screen Recording Guide](https://cs1230.graphics/docs/screen-recording/) for machine-specific guidance on how to do this.

Once you've recorded everything, navigate to this file in Github, click edit, and either select or drag-and-drop each of your videos into the correct location. This will upload them to GitHub (but not include them in the repo) and automatically embed them into this Markdown file by providing a link. Make sure to double-check that they all show up properly in the preview.

> Note: you can ignore the `student_outputs` folder in the repo for now. You will be using it in the next project, Filter.

We're **not** looking for your video to **exactly** match the expected output (i.e. draw the exact same shape). Just make sure to follow the instructions and verify that the recording fully demonstrates the functionality of that part of your project.

### Constant Brush
**Instructions:** Draw with the constant brush.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/241d99b5-947a-407c-b454-410534520aad

#### Your Output

https://github.com/vli18/projects-brush-vli18/blob/main/videos/constant%20brush.mov

### Linear Brush
**Instructions:** Draw with the linear brush.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/9706fa04-7710-441f-b292-ab010e04dec6

#### Your Output

https://github.com/vli18/projects-brush-vli18/blob/main/videos/linear%20brush.mov

### Quadratic Brush
**Instructions:** Draw with the quadratic brush.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/c5df5c09-bfe0-4c05-a56e-14609772d675

#### Your Output

https://github.com/vli18/projects-brush-vli18/blob/main/videos/quadratic%20brush.mov

### Smudge Brush
**Instructions:** Draw some colors on the canvas and use the smudge brush to smear them together.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/26440b63-2d1c-43fd-95f2-55b74ad3bbed

#### Your Output

https://github.com/vli18/projects-brush-vli18/blob/main/videos/smudge%20brush.mov


### Smudge Brush Change in Alpha
**Instructions:** Draw some colors on the canvas. Use the smudge brush with varying alpha levels (use at least three) and demonstrate that the brush still works the same way each time.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/0b49c7d0-47ca-46d0-af72-48b831dfe7ea

#### Your Output

https://github.com/vli18/projects-brush-vli18/blob/main/videos/smudge%20alpha.mov

### Radius
**Instructions:** Use any brush with at least 3 different values for the radius.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/6f619df6-80cd-4849-8831-6a5aade2a517

#### Your Output

https://github.com/vli18/projects-brush-vli18/blob/main/videos/brush%20radius.mov


### Color
**Instructions:** Use any brush to draw red (255, 0, 0), green (0, 255, 0), and blue (0, 0, 255).

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/fd9578ca-e0af-433e-ac9e-b27db2ceebc9

#### Your Output

https://github.com/vli18/projects-brush-vli18/blob/main/videos/brush%20color.mov


### Canvas Edge Behavior
**Instructions:** With any brush, click and draw on the canvas in a place where the mask intersects with the edge. Then, start drawing anywhere on the canvas and drag your mouse off of the edge.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/f5344248-fa5f-4c33-b6df-ff0a45011c7a

#### Your Output

https://github.com/vli18/projects-brush-vli18/blob/main/videos/canvas%20edge.mov

### Alpha
**Instructions:** With the constant brush, draw a single dot of red (255, 0, 0) with an alpha of 255. Then, draw over it with a single dot of blue (0, 0, 255) with an alpha of 100. You should get a purpleish color.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/b13d312d-d6d4-4375-aeaa-96174065443b

#### Your Output

https://github.com/vli18/projects-brush-vli18/blob/main/videos/alpha.mov


### Alpha of Zero
**Instructions:** Choose any brush and demonstrate that it will not draw if the alpha value is zero.

#### Expected Output

https://github.com/BrownCSCI1230/projects_raster_template/assets/77859770/8e48777e-8196-401e-9af6-871abe712146

#### Your Output

https://github.com/vli18/projects-brush-vli18/blob/main/videos/alpha%20zero.mov

## Design Choices
I grouped my different brush mask implementations into one function applyBrush() which uses the brush type
to calculate different mask values. The color function determines the final color result of the brush given
its desired RGBA values. Smudge brush also requires a pick up color function that updates the temporary data
for the brush. Spray brush uses random values to generate the spray effect within a radius value with desired
spray density. Fill bucket uses a queue to store pixel information and checks for neighboring pixels to determine
the fill area and retrieve and apply color information. 

## Collaboration/References
ChatGPT was used in helping with the color blending code and some debugging for the brush masks.

## Known Bugs

## Extra Credit
I implemented the Spray Brush and Fill Bucket as my 8 points of extra credit for CSCI1234. 
1. The code for Spray Brush can be found in sprayBrush() and is called in the mouseDown and mouseUp functions
2. The code for Fill Bucket can be found in fillBucket() and is called in the mouseDown function

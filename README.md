# 3D Graphics Programming
<p align="center">
  <img src="https://github.com/mknightdev/3DGP_Assignment/assets/44908232/89691d4c-61ab-40b1-8084-2b63113ae16d.png" width=50% height=50%>
</p>

## About
<p>In this assignment, I had the choice of creating a 3D game, tool, or tech demo to demonstrate my understanding of 3D graphics and programming principles. I decided to create a 3D Model Viewer, which could handle custom models and their textures using the text file provided.</p>

## Usage
<p>A .txt file named "Model_Settings" <b>must</b> be located in the same directory as the .exe. The file path of the model should be first, followed by its texture. Please see the example below.</p>

### Example

```
models/skeleton/skeleton.obj
models/skeleton/skeleton_diffuse.png
models/croc/croc.obj
models/croc/croc_diffuse.png
models/imp/imp.obj
models/imp/imp_diffuse.png
```

## Lighting 
<p>Three types of lighting were implemented (in order of appearance): diffuse, ambient and specular.</p>

<p float="left">
  <img src="https://user-images.githubusercontent.com/44908232/192143896-e69b3aa9-8491-4378-87a1-d22c14e4899b.png" width=32% />
  <img src="https://user-images.githubusercontent.com/44908232/192144115-88ca0f33-0409-4cfa-9cae-79b381a2d902.png" width=32% />
  <img src="https://user-images.githubusercontent.com/44908232/192144138-50d93978-981d-477f-8a15-400b31de339e.png" width=32% />
</p>

## UI Elements
### Texture
<img src="https://user-images.githubusercontent.com/44908232/192144335-90eb36db-ecf2-4223-b250-85b17b5a7065.png" width=10% height=10%>
<p>The top-left of the application displays the texture of the current model.</p>

### Scaling
<img src="https://user-images.githubusercontent.com/44908232/192144227-e47983a1-4fd0-4e47-add5-388ed343112c.png" width=10% height=10%>
<p>In case the custom models loaded were large, I implemented scaling buttons to increase and decrease the model.</p> 

### Rotation
<img src="https://github.com/mknightdev/3DGP_Assignment/assets/44908232/fc85dae3-e881-4c60-a269-a78024236548.png" width=10% height=10%>
<p>The rotation button enables and disables the model rotation. If it is disabled, the model will face the camera.</p>

### Undo
<img src="https://github.com/mknightdev/3DGP_Assignment/assets/44908232/7d2fa588-5eff-4d06-9c87-25828d7f0773.png" width=10% height=10%>
<p>The undo button resets the scale, and the lighting of the model back to their defaults.</p> 

### Model Select
<img src="https://github.com/mknightdev/3DGP_Assignment/assets/44908232/c06865ed-ce52-44f1-a92f-467ff9a5e5ee.png" width=15% height=15%>
<p>To cycle through the loaded models and textures, use the arrows in the bottom left of the application to cycle through them.</p>

### Lighting Select
<img src="https://github.com/mknightdev/3DGP_Assignment/assets/44908232/a754a07b-8d5c-46ea-be38-599d7c399937.png" width=15% height=15%>
<p>To cycle through the lighting options, use the arrows in the bottom right of the application to cycle through them.</p>

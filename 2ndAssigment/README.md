<h1> Instruction for execution </h1>

<ul>
    <li>
        <h2>Linux/use of g++:</h2>
        <ul>
            <li>Install package "libassimp-dev" using the command "apt-get";</li>
            <li>Download the repository, enter the folder src, use the command ```make && ./lab.out```</li>
        </ul>
    </li>
    <li>
        <h2>Visual Studio:</li>
        <ul>
            <li>Create a new empty C++ project and place the downloaded folder in the internal project folder (the one at the same level as the .sln file);</li>
            <li>Install via the "Nuget Package Management Console" the packages "nupengl.core" and "glm" ("install-package" command);</li>
            <li>Make sure you have the libraries for assimp and freeType installed; otherwise you need to delete #include freeType either in the source code and in the Makefile</li>
            <li>Select the mode <strong>Release</strong> e <strong>x64</strong>;</li>
            <li>In the project settings, include the library<strong>"assimp-vc142-mt.lib"</strong>and set the build version<strong>c++17</strong>;</li>
            <li>Add all .h, .hpp and .cpp files to the project from the "Solution Explorer" sidebar, by clicking respectively on "Header Files" for .h files and "Source Files" for .cpp files (right click -> Add -> Existing item);</li>
            <li>Open the "Lab_Texture_MeshObj.cpp" file</li>
            <li>Run the project using Visual Studio.</li>
        </ul>
    </li>
</ul>

<h1> Commands to control the airplane: </h1>

<ul>
    <li>b to switch between the view of original space and the airplane's view</li>
    <li>j to turn left</li>
    <li>k to turn right</li>
    <li>u to fly high</li>
    <li>i to fly low</li>
    <li>mouse right lick to select objects, then with the middle mouse button you can change shader and material for the selected objects</li>
    <li>mouse left click to rotate arong the airplane</li>
    <li>W,S,A,D to move aroung the camera</li>
</ul>

The objective of this game is to fly forever, you are free to control the airplane to move forward.

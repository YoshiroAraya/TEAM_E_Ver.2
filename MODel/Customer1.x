xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 145;
 -17.89484;11.62598;-0.02674;,
 0.02674;57.85083;-0.02674;,
 -12.64574;11.62598;-12.69922;,
 0.02674;57.85083;-0.02674;,
 0.02674;11.62598;-17.94832;,
 0.02674;57.85083;-0.02674;,
 12.69922;11.62598;-12.69922;,
 0.02674;57.85083;-0.02674;,
 17.94832;11.62598;-0.02674;,
 0.02674;57.85083;-0.02674;,
 12.69922;11.62598;12.64574;,
 0.02674;57.85083;-0.02674;,
 0.02674;11.62598;17.89484;,
 0.02674;57.85083;-0.02674;,
 -12.64574;11.62598;12.64574;,
 0.02674;57.85083;-0.02674;,
 -17.89484;11.62598;-0.02674;,
 0.02674;-0.12500;-0.02674;,
 -17.89484;11.62598;-0.02674;,
 -12.64574;11.62598;-12.69922;,
 0.02674;11.62598;-17.94832;,
 12.69922;11.62598;-12.69922;,
 17.94832;11.62598;-0.02674;,
 12.69922;11.62598;12.64574;,
 0.02674;11.62598;17.89484;,
 -12.64574;11.62598;12.64574;,
 0.02674;95.08198;-0.02674;,
 -4.56392;93.79060;-4.61740;,
 -6.46542;93.79060;-0.02674;,
 0.02674;95.08198;-0.02674;,
 0.02674;93.79060;-6.51890;,
 0.02674;95.08198;-0.02674;,
 4.61740;93.79060;-4.61740;,
 0.02674;95.08198;-0.02674;,
 6.51890;93.79060;-0.02674;,
 0.02674;95.08198;-0.02674;,
 4.61740;93.79060;4.56392;,
 0.02674;95.08198;-0.02674;,
 0.02674;93.79060;6.46542;,
 0.02674;95.08198;-0.02674;,
 -4.56392;93.79060;4.56392;,
 0.02674;95.08198;-0.02674;,
 -6.46542;93.79060;-0.02674;,
 -8.45568;90.11310;-8.50916;,
 -11.96922;90.11310;-0.02674;,
 0.02674;90.11310;-12.02270;,
 8.50916;90.11310;-8.50916;,
 12.02270;90.11310;-0.02674;,
 8.50916;90.11310;8.45568;,
 0.02674;90.11310;11.96922;,
 -8.45568;90.11310;8.45568;,
 -11.96922;90.11310;-0.02674;,
 -11.05608;84.60930;-11.10956;,
 -15.64672;84.60930;-0.02674;,
 0.02674;84.60930;-15.70020;,
 11.10956;84.60930;-11.10956;,
 15.70020;84.60930;-0.02674;,
 11.10956;84.60930;11.05608;,
 0.02674;84.60930;15.64672;,
 -11.05608;84.60930;11.05608;,
 -15.64672;84.60930;-0.02674;,
 -11.96922;78.11714;-12.02270;,
 -16.93810;78.11714;-0.02674;,
 0.02674;78.11714;-16.99158;,
 12.02270;78.11714;-12.02270;,
 16.99158;78.11714;-0.02674;,
 12.02270;78.11714;11.96922;,
 0.02674;78.11714;16.93810;,
 -11.96922;78.11714;11.96922;,
 -16.93810;78.11714;-0.02674;,
 -11.05608;71.62498;-11.10956;,
 -15.64672;71.62498;-0.02674;,
 0.02674;71.62498;-15.70020;,
 11.10956;71.62498;-11.10956;,
 15.70020;71.62498;-0.02674;,
 11.10956;71.62498;11.05608;,
 0.02674;71.62498;15.64672;,
 -11.05608;71.62498;11.05608;,
 -15.64672;71.62498;-0.02674;,
 -8.45568;66.12118;-8.50916;,
 -11.96922;66.12118;-0.02674;,
 0.02674;66.12118;-12.02270;,
 8.50916;66.12118;-8.50916;,
 12.02270;66.12118;-0.02674;,
 8.50916;66.12118;8.45568;,
 0.02674;66.12118;11.96922;,
 -8.45568;66.12118;8.45568;,
 -11.96922;66.12118;-0.02674;,
 -4.56392;62.44368;-4.61740;,
 -6.46542;62.44368;-0.02674;,
 0.02674;62.44368;-6.51890;,
 4.61740;62.44368;-4.61740;,
 6.51890;62.44368;-0.02674;,
 4.61740;62.44368;4.56392;,
 0.02674;62.44368;6.46542;,
 -4.56392;62.44368;4.56392;,
 -6.46542;62.44368;-0.02674;,
 0.02674;61.15230;-0.02674;,
 0.02674;61.15230;-0.02674;,
 0.02674;61.15230;-0.02674;,
 0.02674;61.15230;-0.02674;,
 0.02674;61.15230;-0.02674;,
 0.02674;61.15230;-0.02674;,
 0.02674;61.15230;-0.02674;,
 0.02674;61.15230;-0.02674;,
 -17.01024;54.00104;4.10326;,
 -17.01024;54.00104;-4.15674;,
 -32.31758;17.04586;-4.15674;,
 -32.31758;17.04586;4.10326;,
 -17.01024;54.00104;-4.15674;,
 -9.19424;50.76354;-4.15674;,
 -24.50156;13.80836;-4.15674;,
 -32.31758;17.04586;-4.15674;,
 -9.19424;50.76354;-4.15674;,
 -9.19424;50.76354;4.10326;,
 -24.50156;13.80836;4.10326;,
 -24.50156;13.80836;-4.15674;,
 -9.19424;50.76354;4.10326;,
 -17.01024;54.00104;4.10326;,
 -32.31758;17.04586;4.10326;,
 -24.50156;13.80836;4.10326;,
 -17.01024;54.00104;-4.15674;,
 -17.01024;54.00104;4.10326;,
 -32.31758;17.04586;4.10326;,
 -32.31758;17.04586;-4.15674;,
 9.48936;50.59996;4.10326;,
 9.48936;50.59996;-4.15674;,
 24.47362;13.51260;-4.15674;,
 24.47362;13.51260;4.10326;,
 9.48936;50.59996;-4.15674;,
 17.33334;53.76912;-4.15674;,
 32.31758;16.68178;-4.15674;,
 24.47362;13.51260;-4.15674;,
 17.33334;53.76912;-4.15674;,
 17.33334;53.76912;4.10326;,
 32.31758;16.68178;4.10326;,
 32.31758;16.68178;-4.15674;,
 17.33334;53.76912;4.10326;,
 9.48936;50.59996;4.10326;,
 24.47362;13.51260;4.10326;,
 32.31758;16.68178;4.10326;,
 9.48936;50.59996;-4.15674;,
 9.48936;50.59996;4.10326;,
 24.47362;13.51260;4.10326;,
 24.47362;13.51260;-4.15674;;
 
 92;
 3;0,1,2;,
 3;2,3,4;,
 3;4,5,6;,
 3;6,7,8;,
 3;8,9,10;,
 3;10,11,12;,
 3;12,13,14;,
 3;14,15,16;,
 3;17,18,19;,
 3;17,19,20;,
 3;17,20,21;,
 3;17,21,22;,
 3;17,22,23;,
 3;17,23,24;,
 3;17,24,25;,
 3;17,25,18;,
 3;26,27,28;,
 3;29,30,27;,
 3;31,32,30;,
 3;33,34,32;,
 3;35,36,34;,
 3;37,38,36;,
 3;39,40,38;,
 3;41,42,40;,
 4;28,27,43,44;,
 4;27,30,45,43;,
 4;30,32,46,45;,
 4;32,34,47,46;,
 4;34,36,48,47;,
 4;36,38,49,48;,
 4;38,40,50,49;,
 4;40,42,51,50;,
 4;44,43,52,53;,
 4;43,45,54,52;,
 4;45,46,55,54;,
 4;46,47,56,55;,
 4;47,48,57,56;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;53,52,61,62;,
 4;52,54,63,61;,
 4;54,55,64,63;,
 4;55,56,65,64;,
 4;56,57,66,65;,
 4;57,58,67,66;,
 4;58,59,68,67;,
 4;59,60,69,68;,
 4;62,61,70,71;,
 4;61,63,72,70;,
 4;63,64,73,72;,
 4;64,65,74,73;,
 4;65,66,75,74;,
 4;66,67,76,75;,
 4;67,68,77,76;,
 4;68,69,78,77;,
 4;71,70,79,80;,
 4;70,72,81,79;,
 4;72,73,82,81;,
 4;73,74,83,82;,
 4;74,75,84,83;,
 4;75,76,85,84;,
 4;76,77,86,85;,
 4;77,78,87,86;,
 4;80,79,88,89;,
 4;79,81,90,88;,
 4;81,82,91,90;,
 4;82,83,92,91;,
 4;83,84,93,92;,
 4;84,85,94,93;,
 4;85,86,95,94;,
 4;86,87,96,95;,
 3;89,88,97;,
 3;88,90,98;,
 3;90,91,99;,
 3;91,92,100;,
 3;92,93,101;,
 3;93,94,102;,
 3;94,95,103;,
 3;95,96,104;,
 4;105,106,107,108;,
 4;109,110,111,112;,
 4;113,114,115,116;,
 4;117,118,119,120;,
 4;117,110,121,122;,
 4;123,124,111,120;,
 4;125,126,127,128;,
 4;129,130,131,132;,
 4;133,134,135,136;,
 4;137,138,139,140;,
 4;137,130,141,142;,
 4;143,144,131,140;;
 
 MeshMaterialList {
  2;
  92;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.793726;0.000000;0.021961;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.533600;0.254400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  87;
  -0.932377;0.361487;0.000000;,
  -0.659290;0.361487;-0.659290;,
  -0.000000;0.361487;-0.932377;,
  0.659290;0.361487;-0.659290;,
  0.932377;0.361487;-0.000000;,
  0.659290;0.361487;0.659290;,
  0.000000;0.361487;0.932377;,
  -0.659290;0.361487;0.659290;,
  0.000000;-1.000000;-0.000000;,
  -0.548328;-0.836263;0.000000;,
  -0.387726;-0.836263;-0.387726;,
  -0.000000;-0.836263;-0.548328;,
  0.387726;-0.836263;-0.387726;,
  0.548328;-0.836263;-0.000000;,
  0.387726;-0.836263;0.387726;,
  0.000000;-0.836263;0.548328;,
  -0.387726;-0.836263;0.387726;,
  0.000000;1.000000;0.000000;,
  -0.380611;0.924735;-0.000000;,
  -0.269132;0.924735;-0.269132;,
  0.000000;0.924735;-0.380611;,
  0.269132;0.924735;-0.269132;,
  0.380611;0.924735;0.000000;,
  0.269132;0.924735;0.269132;,
  -0.000000;0.924735;0.380611;,
  -0.269132;0.924735;0.269132;,
  -0.704977;0.709230;-0.000000;,
  -0.498494;0.709231;-0.498494;,
  0.000000;0.709230;-0.704977;,
  0.498494;0.709231;-0.498494;,
  0.704977;0.709230;0.000000;,
  0.498494;0.709231;0.498494;,
  -0.000000;0.709230;0.704977;,
  -0.498494;0.709231;0.498494;,
  -0.923104;0.384551;0.000000;,
  -0.652733;0.384551;-0.652733;,
  -0.000000;0.384551;-0.923104;,
  0.652733;0.384551;-0.652733;,
  0.923104;0.384551;-0.000000;,
  0.652733;0.384551;0.652733;,
  0.000000;0.384551;0.923104;,
  -0.652733;0.384551;0.652733;,
  -1.000000;-0.000000;0.000000;,
  -0.707107;-0.000000;-0.707107;,
  -0.000000;-0.000000;-1.000000;,
  0.707107;-0.000000;-0.707107;,
  1.000000;-0.000000;-0.000000;,
  0.707107;-0.000000;0.707107;,
  0.000000;-0.000000;1.000000;,
  -0.707107;-0.000000;0.707107;,
  -0.923104;-0.384550;0.000000;,
  -0.652733;-0.384551;-0.652733;,
  -0.000000;-0.384550;-0.923104;,
  0.652733;-0.384551;-0.652733;,
  0.923104;-0.384550;-0.000000;,
  0.652733;-0.384551;0.652733;,
  0.000000;-0.384550;0.923104;,
  -0.652733;-0.384551;0.652733;,
  -0.704977;-0.709231;-0.000000;,
  -0.498494;-0.709231;-0.498494;,
  0.000000;-0.709231;-0.704977;,
  0.498494;-0.709231;-0.498494;,
  0.704977;-0.709231;0.000000;,
  0.498494;-0.709231;0.498494;,
  -0.000000;-0.709231;0.704977;,
  -0.498494;-0.709231;0.498494;,
  -0.380611;-0.924735;-0.000000;,
  -0.269132;-0.924736;-0.269132;,
  0.000000;-0.924735;-0.380611;,
  0.269132;-0.924736;-0.269132;,
  0.380611;-0.924735;0.000000;,
  0.269132;-0.924736;0.269132;,
  -0.000000;-0.924735;0.380611;,
  -0.269132;-0.924736;0.269132;,
  0.000000;-1.000000;0.000000;,
  -0.923880;0.382684;0.000000;,
  -0.000000;-0.000000;-1.000000;,
  0.923880;-0.382683;-0.000000;,
  0.000000;0.000000;1.000000;,
  0.382684;0.923879;0.000000;,
  -0.382683;-0.923880;0.000000;,
  -0.927184;-0.374606;0.000000;,
  -0.000000;0.000000;-1.000000;,
  0.927184;0.374606;0.000000;,
  0.000000;-0.000000;1.000000;,
  -0.374605;0.927184;0.000000;,
  0.374608;-0.927183;-0.000000;;
  92;
  3;0,1,1;,
  3;1,1,2;,
  3;2,3,3;,
  3;3,3,4;,
  3;4,5,5;,
  3;5,5,6;,
  3;6,7,7;,
  3;7,7,0;,
  3;8,9,10;,
  3;8,10,11;,
  3;8,11,12;,
  3;8,12,13;,
  3;8,13,14;,
  3;8,14,15;,
  3;8,15,16;,
  3;8,16,9;,
  3;17,19,18;,
  3;17,20,19;,
  3;17,21,20;,
  3;17,22,21;,
  3;17,23,22;,
  3;17,24,23;,
  3;17,25,24;,
  3;17,18,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,24,32,31;,
  4;24,25,33,32;,
  4;25,18,26,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,33,41,40;,
  4;33,26,34,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,41,49,48;,
  4;41,34,42,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,49,57,56;,
  4;49,42,50,57;,
  4;50,51,59,58;,
  4;51,52,60,59;,
  4;52,53,61,60;,
  4;53,54,62,61;,
  4;54,55,63,62;,
  4;55,56,64,63;,
  4;56,57,65,64;,
  4;57,50,58,65;,
  4;58,59,67,66;,
  4;59,60,68,67;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,63,71,70;,
  4;63,64,72,71;,
  4;64,65,73,72;,
  4;65,58,66,73;,
  3;66,67,74;,
  3;67,68,74;,
  3;68,69,74;,
  3;69,70,74;,
  3;70,71,74;,
  3;71,72,74;,
  3;72,73,74;,
  3;73,66,74;,
  4;75,75,75,75;,
  4;76,76,76,76;,
  4;77,77,77,77;,
  4;78,78,78,78;,
  4;79,79,79,79;,
  4;80,80,80,80;,
  4;81,81,81,81;,
  4;82,82,82,82;,
  4;83,83,83,83;,
  4;84,84,84,84;,
  4;85,85,85,85;,
  4;86,86,86,86;;
 }
 MeshTextureCoords {
  145;
  0.000000;1.000000;,
  0.062500;0.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.187500;0.000000;,
  0.375000;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.312500;0.000000;,
  0.625000;1.000000;,
  0.375000;0.000000;,
  0.750000;1.000000;,
  0.437500;0.000000;,
  0.875000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}

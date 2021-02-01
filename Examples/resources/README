General:

Files in xcf/ folder are Gimp files made with version 2.8. Use this version to take advantage of layer groups in the image. These images are a kind of source image. The file male_sprites.xcf contains all the different animations, including the animations walk, slash, hurt, spellcast from base lpc assets, as well as bow and thrust animations which are made by me.

Images in png/ folder are all sprite animation sheets in 64x64 px grids. Files are sorted by animation type in separate folders, named in convention BODYPART_part_name. In some cases part name contains suggestion for use as a kit, e.g. "leather_armor", "robe", "plate_armor", "chain_armor", but many more combinations are possible.
The files BODY_male in folders hurt, slash, spellcast, walkcycle, are obviously part of the base lpc assets and are only provided for completeness.

The folder examples/ contains animated gif images which demonstrates some different clothes, equipment and color combinations. You can preview them easily in a web browser or image manipulation program.

Note that the skeleton animation sheet is compatible with the clothes and equipment sprites, but the combat dummy animation is not.


Animation sequence information:

Bow animation:
  1-9 Raise bow and aim
  10 Loose arrow
  11-13 Grab new arrow
  5-9 Aim new arrow
  10 Loose new arrow

Thrust animation:
  1-4 Raise staff
  5-8 Thrust loop.

Skeleton (and male) walk animation:
  1 Stand/idle
  2-9 Walkcycle


Sprite order information:

The best order to place sprites in top to bottom is visible in the xcf files:
  1. WEAPON
  2. HANDS
  3. HEAD
  4. BELT
  5. TORSO
  6. LEGS
  7. FEET
  8. BODY
  9. BEHIND


Gif animation tips:

To create an animated gif from any of the sprite sheets use ImageMagic and issue these commands:

Cut the sheet into separate image files:
convert some_sheet_name.png -crop 64x64 +repage +adjoin tile_%02d.png

Then join the images into a gif:
convert -delay 12 -loop 0 -dispose Background tile_*.png animation.gif


Contact:

Johannes Sjölund

j.sjolund@gmail.com

Freenode: wulax

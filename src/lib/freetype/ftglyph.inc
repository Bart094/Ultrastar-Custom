(***************************************************************************)
(*                                                                         *)
(*  ftglyph.h                                                              *)
(*                                                                         *)
(*    FreeType convenience functions to handle glyphs (specification).     *)
(*                                                                         *)
(*  Copyright 1996-2001, 2002, 2003, 2006 by                               *)
(*  David Turner, Robert Wilhelm, and Werner Lemberg.                      *)
(*                                                                         *)
(*  This file is part of the FreeType project, and may only be used,       *)
(*  modified, and distributed under the terms of the FreeType project      *)
(*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     *)
(*  this file you indicate that you have read the license and              *)
(*  understand and accept it fully.                                        *)
(*                                                                         *)
(***************************************************************************)
(***************************************************************************)
(* Pascal port by the UltraStar Deluxe Team                                *)
(***************************************************************************)


  (*************************************************************************)
  (*                                                                       *)
  (* This file contains the definition of several convenience functions    *)
  (* that can be used by client applications to easily retrieve glyph      *)
  (* bitmaps and outlines from a given face.                               *)
  (*                                                                       *)
  (* These functions should be optional if you are writing a font server   *)
  (* or text layout engine on top of FreeType.  However, they are pretty   *)
  (* handy for many other simple uses of the library.                      *)
  (*                                                                       *)
  (*************************************************************************)

  (*************************************************************************)
  (*                                                                       *)
  (* <Section>                                                             *)
  (*    glyph_management                                                   *)
  (*                                                                       *)
  (* <Title>                                                               *)
  (*    Glyph Management                                                   *)
  (*                                                                       *)
  (* <Abstract>                                                            *)
  (*    Generic interface to manage individual glyph data.                 *)
  (*                                                                       *)
  (* <Description>                                                         *)
  (*    This section contains definitions used to manage glyph data        *)
  (*    through generic FT_Glyph objects.  Each of them can contain a      *)
  (*    bitmap, a vector outline, or even images in other formats.         *)
  (*                                                                       *)
  (*************************************************************************)

{$IFDEF TYPE_DECL}
  
  (* forward declaration to a private type *)
  PFT_Glyph_Class = Pointer;

  
  (*************************************************************************)
  (*                                                                       *)
  (* <Type>                                                                *)
  (*    FT_Glyph                                                           *)
  (*                                                                       *)
  (* <Description>                                                         *)
  (*    Handle to an object used to model generic glyph images.  It is a   *)
  (*    pointer to the @FT_GlyphRec structure and can contain a glyph      *)
  (*    bitmap or pointer.                                                 *)
  (*                                                                       *)
  (* <Note>                                                                *)
  (*    Glyph objects are not owned by the library.  You must thus release *)
  (*    them manually (through @FT_Done_Glyph) _before_ calling            *)
  (*    @FT_Done_FreeType.                                                 *)
  (*                                                                       *)
  FT_Glyph = ^FT_GlyphRec;
  
  (*************************************************************************)
  (*                                                                       *)
  (* <Struct>                                                              *)
  (*    FT_GlyphRec                                                        *)
  (*                                                                       *)
  (* <Description>                                                         *)
  (*    The root glyph structure contains a given glyph image plus its     *)
  (*    advance width in 16.16 fixed float format.                         *)
  (*                                                                       *)
  (* <Fields>                                                              *)
  (*    library :: A handle to the FreeType library object.                *)
  (*                                                                       *)
  (*    clazz   :: A pointer to the glyph's class.  Private.               *)
  (*                                                                       *)
  (*    format  :: The format of the glyph's image.                        *)
  (*                                                                       *)
  (*    advance :: A 16.16 vector that gives the glyph's advance width.    *)
  (*                                                                       *)
  FT_GlyphRec = record
    library_: FT_Library;
    clazz:    PFT_Glyph_Class;
    format:   FT_Glyph_Format;
    advance:  FT_Vector;
  end;


  (*************************************************************************)
  (*                                                                       *)
  (* <Type>                                                                *)
  (*    FT_BitmapGlyph                                                     *)
  (*                                                                       *)
  (* <Description>                                                         *)
  (*    A handle to an object used to model a bitmap glyph image.  This is *)
  (*    a sub-class of @FT_Glyph, and a pointer to @FT_BitmapGlyphRec.     *)
  (*                                                                       *)
  FT_BitmapGlyph = ^FT_BitmapGlyphRec;

  (*************************************************************************)
  (*                                                                       *)
  (* <Struct>                                                              *)
  (*    FT_BitmapGlyphRec                                                  *)
  (*                                                                       *)
  (* <Description>                                                         *)
  (*    A structure used for bitmap glyph images.  This really is a        *)
  (*    `sub-class' of `FT_GlyphRec'.                                      *)
  (*                                                                       *)
  (* <Fields>                                                              *)
  (*    root   :: The root FT_Glyph fields.                                *)
  (*                                                                       *)
  (*    left   :: The left-side bearing, i.e., the horizontal distance     *)
  (*              from the current pen position to the left border of the  *)
  (*              glyph bitmap.                                            *)
  (*                                                                       *)
  (*    top    :: The top-side bearing, i.e., the vertical distance from   *)
  (*              the current pen position to the top border of the glyph  *)
  (*              bitmap.  This distance is positive for upwards-y!        *)
  (*                                                                       *)
  (*    bitmap :: A descriptor for the bitmap.                             *)
  (*                                                                       *)
  (* <Note>                                                                *)
  (*    You can typecast FT_Glyph to FT_BitmapGlyph if you have            *)
  (*    glyph->format == FT_GLYPH_FORMAT_BITMAP.  This lets you access     *)
  (*    the bitmap's contents easily.                                      *)
  (*                                                                       *)
  (*    The corresponding pixel buffer is always owned by the BitmapGlyph  *)
  (*    and is thus created and destroyed with it.                         *)
  (*                                                                       *)
  FT_BitmapGlyphRec = record
    root:   FT_GlyphRec;
    left:   FT_Int;
    top:    FT_Int;
    bitmap: FT_Bitmap;
  end;


  (*************************************************************************)
  (*                                                                       *)
  (* <Type>                                                                *)
  (*    FT_OutlineGlyph                                                    *)
  (*                                                                       *)
  (* <Description>                                                         *)
  (*    A handle to an object used to model an outline glyph image.  This  *)
  (*    is a sub-class of @FT_Glyph, and a pointer to @FT_OutlineGlyphRec. *)
  (*                                                                       *)
  FT_OutlineGlyph = ^FT_OutlineGlyphRec;


  (*************************************************************************)
  (*                                                                       *)
  (* <Struct>                                                              *)
  (*    FT_OutlineGlyphRec                                                 *)
  (*                                                                       *)
  (* <Description>                                                         *)
  (*    A structure used for outline (vectorial) glyph images.  This       *)
  (*    really is a `sub-class' of `FT_GlyphRec'.                          *)
  (*                                                                       *)
  (* <Fields>                                                              *)
  (*    root    :: The root FT_Glyph fields.                               *)
  (*                                                                       *)
  (*    outline :: A descriptor for the outline.                           *)
  (*                                                                       *)
  (* <Note>                                                                *)
  (*    You can typecast FT_Glyph to FT_OutlineGlyph if you have           *)
  (*    glyph->format == FT_GLYPH_FORMAT_OUTLINE.  This lets you access    *)
  (*    the outline's content easily.                                      *)
  (*                                                                       *)
  (*    As the outline is extracted from a glyph slot, its coordinates are *)
  (*    expressed normally in 26.6 pixels, unless the flag                 *)
  (*    FT_LOAD_NO_SCALE was used in FT_Load_Glyph() or FT_Load_Char().    *)
  (*                                                                       *)
  (*    The outline's tables are always owned by the object and are        *)
  (*    destroyed with it.                                                 *)
  (*                                                                       *)
  FT_OutlineGlyphRec = record
    root:    FT_GlyphRec;
    outline: FT_Outline;
  end;

{$ELSE TYPE_DECL}

  (*************************************************************************)
  (*                                                                       *)
  (* <Function>                                                            *)
  (*    FT_Get_Glyph                                                       *)
  (*                                                                       *)
  (* <Description>                                                         *)
  (*    A function used to extract a glyph image from a slot.              *)
  (*                                                                       *)
  (* <Input>                                                               *)
  (*    slot   :: A handle to the source glyph slot.                       *)
  (*                                                                       *)
  (* <Output>                                                              *)
  (*    aglyph :: A handle to the glyph object.                            *)
  (*                                                                       *)
  (* <Return>                                                              *)
  (*    FreeType error code.  0 means success.                             *)
  (*                                                                       *)
  function FT_Get_Glyph(
             slot:   FT_GlyphSlot;
             out aglyph: FT_Glyph ): FT_Error;
    cdecl; external ft_lib name 'FT_Get_Glyph';

  (*************************************************************************)
  (*                                                                       *)
  (* <Function>                                                            *)
  (*    FT_Glyph_Copy                                                      *)
  (*                                                                       *)
  (* <Description>                                                         *)
  (*    A function used to copy a glyph image.  Note that the created      *)
  (*    @FT_Glyph object must be released with @FT_Done_Glyph.             *)
  (*                                                                       *)
  (* <Input>                                                               *)
  (*    source :: A handle to the source glyph object.                     *)
  (*                                                                       *)
  (* <Output>                                                              *)
  (*    target :: A handle to the target glyph object.  0~in case of       *)
  (*              error.                                                   *)
  (*                                                                       *)
  (* <Return>                                                              *)
  (*    FreeType error code.  0~means success.                             *)
  (*                                                                       *)
  function FT_Glyph_Copy(source: FT_Glyph;
                 var target: FT_Glyph ): FT_Error;
    cdecl; external ft_lib name 'FT_Glyph_Copy';

{$ENDIF TYPE_DECL}
{$IFDEF TYPE_DECL}

  (*************************************************************************)
  (*                                                                       *)
  (* <Enum>                                                                *)
  (*    FT_Glyph_BBox_Mode                                                 *)
  (*                                                                       *)
  (* <Description>                                                         *)
  (*    The mode how the values of @FT_Glyph_Get_CBox are returned.        *)
  (*                                                                       *)
  (* <Values>                                                              *)
  (*    FT_GLYPH_BBOX_UNSCALED ::                                          *)
  (*      Return unscaled font units.                                      *)
  (*                                                                       *)
  (*    FT_GLYPH_BBOX_SUBPIXELS ::                                         *)
  (*      Return unfitted 26.6 coordinates.                                *)
  (*                                                                       *)
  (*    FT_GLYPH_BBOX_GRIDFIT ::                                           *)
  (*      Return grid-fitted 26.6 coordinates.                             *)
  (*                                                                       *)
  (*    FT_GLYPH_BBOX_TRUNCATE ::                                          *)
  (*      Return coordinates in integer pixels.                            *)
  (*                                                                       *)
  (*    FT_GLYPH_BBOX_PIXELS ::                                            *)
  (*      Return grid-fitted pixel coordinates.                            *)
  (*                                                                       *)
  FT_Glyph_BBox_Mode = cint;
{$ELSE TYPE_DECL}
const
  FT_GLYPH_BBOX_UNSCALED  = 0;
  FT_GLYPH_BBOX_SUBPIXELS = 0;
  FT_GLYPH_BBOX_GRIDFIT   = 1;
  FT_GLYPH_BBOX_TRUNCATE  = 2;
  FT_GLYPH_BBOX_PIXELS    = 3;


  (*************************************************************************)
  (*                                                                       *)
  (* <Function>                                                            *)
  (*    FT_Glyph_Get_CBox                                                  *)
  (*                                                                       *)
  (* <Description>                                                         *)
  (*    Return a glyph's `control box'.  The control box encloses all the  *)
  (*    outline's points, including B?zier control points.  Though it      *)
  (*    coincides with the exact bounding box for most glyphs, it can be   *)
  (*    slightly larger in some situations (like when rotating an outline  *)
  (*    which contains B?zier outside arcs).                               *)
  (*                                                                       *)
  (*    Computing the control box is very fast, while getting the bounding *)
  (*    box can take much more time as it needs to walk over all segments  *)
  (*    and arcs in the outline.  To get the latter, you can use the       *)
  (*    `ftbbox' component which is dedicated to this single task.         *)
  (*                                                                       *)
  (* <Input>                                                               *)
  (*    glyph :: A handle to the source glyph object.                      *)
  (*                                                                       *)
  (*    mode  :: The mode which indicates how to interpret the returned    *)
  (*             bounding box values.                                      *)
  (*                                                                       *)
  (* <Output>                                                              *)
  (*    acbox :: The glyph coordinate bounding box.  Coordinates are       *)
  (*             expressed in 1/64th of pixels if it is grid-fitted.       *)
  (*                                                                       *)
  (* <Note>                                                                *)
  (*    Coordinates are relative to the glyph origin, using the Y-upwards  *)
  (*    convention.                                                        *)
  (*                                                                       *)
  (*    If the glyph has been loaded with @FT_LOAD_NO_SCALE, `bbox_mode'   *)
  (*    must be set to @FT_GLYPH_BBOX_UNSCALED to get unscaled font        *)
  (*    units in 26.6 pixel format.  The value @FT_GLYPH_BBOX_SUBPIXELS    *)
  (*    is another name for this constant.                                 *)
  (*                                                                       *)
  (*    Note that the maximum coordinates are exclusive, which means that  *)
  (*    one can compute the width and height of the glyph image (be it in  *)
  (*    integer or 26.6 pixels) as:                                        *)
  (*                                                                       *)
  (*    {                                                                  *)
  (*      width  = bbox.xMax - bbox.xMin;                                  *)
  (*      height = bbox.yMax - bbox.yMin;                                  *)
  (*    }                                                                  *)
  (*                                                                       *)
  (*    Note also that for 26.6 coordinates, if `bbox_mode' is set to      *)
  (*    @FT_GLYPH_BBOX_GRIDFIT, the coordinates will also be grid-fitted,  *)
  (*    which corresponds to:                                              *)
  (*                                                                       *)
  (*    {                                                                  *)
  (*      bbox.xMin = FLOOR(bbox.xMin);                                    *)
  (*      bbox.yMin = FLOOR(bbox.yMin);                                    *)
  (*      bbox.xMax = CEILING(bbox.xMax);                                  *)
  (*      bbox.yMax = CEILING(bbox.yMax);                                  *)
  (*    }                                                                  *)
  (*                                                                       *)
  (*    To get the bbox in pixel coordinates, set `bbox_mode' to           *)
  (*    @FT_GLYPH_BBOX_TRUNCATE.                                           *)
  (*                                                                       *)
  (*    To get the bbox in grid-fitted pixel coordinates, set `bbox_mode'  *)
  (*    to @FT_GLYPH_BBOX_PIXELS.                                          *)
  (*                                                                       *)
  procedure FT_Glyph_Get_CBox( glyph: FT_Glyph;
                     bbox_mode: FT_UInt;
                     out acbox: FT_BBox );
    cdecl; external ft_lib name 'FT_Glyph_Get_CBox';


  (*************************************************************************)
  (*                                                                       *)
  (* <Function>                                                            *)
  (*    FT_Glyph_To_Bitmap                                                 *)
  (*                                                                       *)
  (* <Description>                                                         *)
  (*    Converts a given glyph object to a bitmap glyph object.            *)
  (*                                                                       *)
  (* <InOut>                                                               *)
  (*    the_glyph   :: A pointer to a handle to the target glyph.          *)
  (*                                                                       *)
  (* <Input>                                                               *)
  (*    render_mode :: An enumeration that describe how the data is        *)
  (*                   rendered.                                           *)
  (*                                                                       *)
  (*    origin      :: A pointer to a vector used to translate the glyph   *)
  (*                   image before rendering.  Can be 0 (if no            *)
  (*                   translation).  The origin is expressed in           *)
  (*                   26.6 pixels.                                        *)
  (*                                                                       *)
  (*    destroy     :: A boolean that indicates that the original glyph    *)
  (*                   image should be destroyed by this function.  It is  *)
  (*                   never destroyed in case of error.                   *)
  (*                                                                       *)
  (* <Return>                                                              *)
  (*    FreeType error code.  0 means success.                             *)
  (*                                                                       *)
  (* <Note>                                                                *)
  (*    The glyph image is translated with the `origin' vector before      *)
  (*    rendering.                                                         *)
  (*                                                                       *)
  (*    The first parameter is a pointer to a FT_Glyph handle, that will   *)
  (*    be replaced by this function.  Typically, you would use (omitting  *)
  (*    error handling):                                                   *)
  (*                                                                       *)
  (*                                                                       *)
  (*      {                                                                *)
  (*        FT_Glyph        glyph;                                         *)
  (*        FT_BitmapGlyph  glyph_bitmap;                                  *)
  (*                                                                       *)
  (*                                                                       *)
  (*        // load glyph                                                  *)
  (*        error = FT_Load_Char( face, glyph_index, FT_LOAD_DEFAUT );     *)
  (*                                                                       *)
  (*        // extract glyph image                                         *)
  (*        error = FT_Get_Glyph( face->glyph, &glyph );                   *)
  (*                                                                       *)
  (*        // convert to a bitmap (default render mode + destroy old)     *)
  (*        if ( glyph->format != FT_GLYPH_FORMAT_BITMAP )                 *)
  (*        {                                                              *)
  (*          error = FT_Glyph_To_Bitmap( &glyph, FT_RENDER_MODE_DEFAULT,  *)
  (*                                      0, 1 );                          *)
  (*          if ( error ) // glyph unchanged                              *)
  (*            ...                                                        *)
  (*        }                                                              *)
  (*                                                                       *)
  (*        // access bitmap content by typecasting                        *)
  (*        glyph_bitmap = (FT_BitmapGlyph)glyph;                          *)
  (*                                                                       *)
  (*        // do funny stuff with it, like blitting/drawing               *)
  (*        ...                                                            *)
  (*                                                                       *)
  (*        // discard glyph image (bitmap or not)                         *)
  (*        FT_Done_Glyph( glyph );                                        *)
  (*      }                                                                *)
  (*                                                                       *)
  (*                                                                       *)
  (*    This function does nothing if the glyph format isn't scalable.     *)
  (*                                                                       *)
  function FT_Glyph_To_Bitmap(var the_glyph: FT_Glyph;
                      render_mode: FT_Render_Mode;
                      origin:      PFT_Vector;
                      destroy:     FT_Bool ): FT_Error;
    cdecl; external ft_lib name 'FT_Glyph_To_Bitmap';

    
  (*************************************************************************)
  (*                                                                       *)
  (* <Function>                                                            *)
  (*    FT_Done_Glyph                                                      *)
  (*                                                                       *)
  (* <Description>                                                         *)
  (*    Destroys a given glyph.                                            *)
  (*                                                                       *)
  (* <Input>                                                               *)
  (*    glyph :: A handle to the target glyph object.                      *)
  (*                                                                       *)
  procedure FT_Done_Glyph( glyph: FT_Glyph );
    cdecl; external ft_lib name 'FT_Done_Glyph';

{$ENDIF TYPE_DECL}

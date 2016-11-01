#include "MakoJPEGLoader.h"
#include "MakoException.h"
#include "MakoTexture.h"
#include "MakoApplication.h"
#include "MakoGraphicsDevice.h"
#include "MakoStream.h"

MAKO_BEGIN_NAMESPACE

void JPEGLoader::init_source(j_decompress_ptr cinfo)
{
	// DO NOTHING
}


boolean JPEGLoader::fill_input_buffer(j_decompress_ptr cinfo)
{
	JPEGLoader* loader = (JPEGLoader*)cinfo->client_data;

	cinfo->src->next_input_byte  = &loader->input;
	loader->istream->ReadTo(&loader->input, 1);
	cinfo->src->bytes_in_buffer  = 1;
	
	return 1;
}


void JPEGLoader::skip_input_data(j_decompress_ptr cinfo, long count)
{
	jpeg_source_mgr* src = cinfo->src;
	if (count > 0)
	{
		//src->bytes_in_buffer -= count;
		//src->next_input_byte += count;
		JPEGLoader* loader = ((JPEGLoader*)cinfo->client_data);

		loader->istream->Skip(count);

		src->bytes_in_buffer = 0;
	}
}


void JPEGLoader::term_source(j_decompress_ptr cinfo)
{
	// DO NOTHING
}


// struct for handling jpeg errors
struct JpegErrorMgr
{
    // public jpeg error fields
    struct jpeg_error_mgr pub;

    // for longjmp, to return to caller on a fatal error
    jmp_buf setjmp_buffer;
};

void JPEGLoader::error_exit(j_common_ptr cinfo)
{
	// unfortunately we need to use a goto rather than throwing an exception
	// as gcc crashes under linux crashes when using throw from within
	// extern c code

	// Always display the message
	(*cinfo->err->output_message) (cinfo);

	// cinfo->err really points to a irr_error_mgr struct
	JpegErrorMgr* myerr = (JpegErrorMgr*) cinfo->err;

	longjmp(myerr->setjmp_buffer, 1);
}


void JPEGLoader::output_message(j_common_ptr cinfo)
{
	// display the error message.
	Int8 temp1[JMSG_LENGTH_MAX];
	(*cinfo->err->format_message)(cinfo, temp1);

	MAKO_DEBUG_BREAK;
}

Texture* JPEGLoader::Load(InputStream* stream)
{
	istream = stream;
	UInt8** rowPtr = nullptr;
	//UInt32 cBytes  = stream->GetSize();
	stream->ReadTo(&input, 1);

	//UInt8* input   = new UInt8[cBytes];
	//stream->ReadTo(input, cBytes);

	// allocate and initialize JPEG decompression object
	jpeg_decompress_struct cinfo;
	JpegErrorMgr jerr;

	cinfo.client_data = this;

	// We have to set up the error handler first, in case the initialization
	// step fails.  (Unlikely, but it could happen if you are out of memory.)
	// This routine fills in the contents of struct jerr, and returns jerr's
	// address which we place into the link field in cinfo.

	cinfo.err                 = jpeg_std_error(&jerr.pub);
	cinfo.err->error_exit     = error_exit;
	cinfo.err->output_message = output_message;

	// compatibility fudge:
	// we need to use setjmp/longjmp for error handling as gcc-linux
	// crashes when throwing within external c code
	if (setjmp(jerr.setjmp_buffer))
	{
		// If we get here, the JPEG code has signaled an error.
		// We need to clean up the JPEG object and return.

		jpeg_destroy_decompress(&cinfo);

		// if the row pointer was created, we delete it.
		if (rowPtr)
			delete [] rowPtr;
		
		// display the error message.
		Int8 temp[JMSG_LENGTH_MAX];
		(*jerr.pub.format_message)((j_common_ptr)&cinfo, temp);
		throw Exception(Text("An error occured in JPEGLoader:\n") + ToString(temp), jerr.pub.msg_code);
	}

	// Now we can initialize the JPEG decompression object.
	jpeg_create_decompress(&cinfo);

	// specify data source
	jpeg_source_mgr jsrc;
	cinfo.src = &jsrc;

	// Set up data pointer
	jsrc.bytes_in_buffer = 1; //cBytes;
	jsrc.next_input_byte = &input;

	jsrc.init_source       = init_source;
	jsrc.fill_input_buffer = fill_input_buffer;
	jsrc.skip_input_data   = skip_input_data;
	jsrc.resync_to_restart = jpeg_resync_to_restart;
	jsrc.term_source       = term_source;

	// Decodes JPG input from whatever source
	// Does everything AFTER jpeg_create_decompress
	// and BEFORE jpeg_destroy_decompress
	// Caller is responsible for arranging these + setting up cinfo

	// read file parameters with jpeg_read_header()
	jpeg_read_header(&cinfo, TRUE);

	cinfo.out_color_space      = JCS_RGB;
	cinfo.out_color_components = 3;
	cinfo.do_fancy_upsampling  = FALSE;

	// Start decompressor
	jpeg_start_decompress(&cinfo);

	// Get image data
	UInt16 rowspan      = cinfo.image_width * cinfo.out_color_components;
	const UInt32 width  = cinfo.image_width;
	const UInt32 height = cinfo.image_height;

	// Allocate memory for buffer
	UInt8* output = new UInt8[rowspan * height];

	// Here we use the library's state variable cinfo.output_scanline as the
	// loop counter, so that we don't have to keep track ourselves.
	// Create array of row pointers for lib
	rowPtr = new UInt8* [height];

	for(UInt32 i = 0; i < height; i++)
		rowPtr[i] = &output[i * rowspan];

	UInt32 rowsRead = 0;

	while (cinfo.output_scanline < cinfo.output_height)
		rowsRead += jpeg_read_scanlines(&cinfo, &rowPtr[rowsRead], cinfo.output_height - rowsRead);
	
	// Clean up
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	delete [] rowPtr;

	// Output is now in R8G8B8 format. We need to convert it to R8G8B8A8
	UInt8* output2 = new UInt8[height * width * (32 / 8)];

	for (UInt8* ptr = output2, *ptr2 = output;
		 ptr != (output2 + (height * width * (32 / 8)));
		 ptr += (32 / 8), ptr2 += (24 / 8))
	{
		//*((UInt32*)ptr) = (255 << 24) | (ptr2[0] << 16) | (ptr2[1] << 8) | ptr2[2];
	
		// Copy the rgb values
		ptr[0] = ptr2[0];
		ptr[1] = ptr2[1];
		ptr[2] = ptr2[2];

		// Set the alpha to 255.
		ptr[3] = 255;
	}

	delete [] output;
	
	TextureCreationParams params;
	params.data   = output2;
	params.size   = Size2d(width, height);
	params.format = CF_R8G8B8A8;

	return APP()->GD()->CreateTexture(params);
}

MAKO_END_NAMESPACE
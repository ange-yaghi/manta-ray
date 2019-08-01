#include <pch.h>

#include "utilities.h"

#include "../include/image_byte_buffer.h"
#include "../include/jpeg_writer.h"
#include "../include/language_rules.h"

void writeToJpeg(const RealMap2D *scalarMap, const std::string &fname) {
    ImageByteBuffer byteBuffer;
    scalarMap->fillByteBuffer(&byteBuffer, true);

    JpegWriter writer;
    writer.setQuality(95);
    writer.write(&byteBuffer, fname.c_str());

    byteBuffer.free();
}

void writeToJpeg(const VectorMap2D *vectorMap, const std::string &fname) {
    ImageByteBuffer byteBuffer;
    vectorMap->fillByteBuffer(&byteBuffer, true);

    JpegWriter writer;
    writer.setQuality(95);
    writer.write(&byteBuffer, fname.c_str());

    byteBuffer.free();
}

void writeToJpeg(const ImagePlane *plane, const std::string &fname) {
    ImageByteBuffer byteBuffer;
    byteBuffer.initialize(plane, true);

    JpegWriter writer;
    writer.setQuality(95);
    writer.write(&byteBuffer, fname.c_str());

    byteBuffer.free();
}

void writeToJpeg(const ComplexMap2D *plane, const std::string &fname, Margins *margins) {
    ImageByteBuffer byteBuffer;
    plane->fillByteBuffer(&byteBuffer, margins);

    JpegWriter writer;
    writer.setQuality(100);
    writer.write(&byteBuffer, fname.c_str());

    byteBuffer.free();
}

piranha::IrCompilationUnit *compileFile(const std::string &filename, const piranha::ErrorList **errList) {
    LanguageRules *rules = new LanguageRules();
    rules->registerBuiltinNodeTypes();

    piranha::Compiler *compiler = new piranha::Compiler(rules);
    piranha::IrCompilationUnit *unit = compiler->compile(SDL_TEST_FILES + filename);
    EXPECT_NE(unit, nullptr);

    if (errList != nullptr) *errList = compiler->getErrorList();

    return unit;
}

// Encode 560x350 BGRA game frames as a silent, 30 FPS H.264 MP4.
import Foundation
import AVFoundation
import CoreVideo

let arguments = CommandLine.arguments
precondition(arguments.count == 3, "Usage: encode_showcase raw-frames output.mp4")
let width = 560, height = 350, fps: Int32 = 30
let output = URL(fileURLWithPath: arguments[2])
let reader = try FileHandle(forReadingFrom: URL(fileURLWithPath: arguments[1]))
let writer = try AVAssetWriter(outputURL: output, fileType: .mp4)
let input = AVAssetWriterInput(mediaType: .video, outputSettings: [
    AVVideoCodecKey: AVVideoCodecType.h264,
    AVVideoWidthKey: width, AVVideoHeightKey: height,
    AVVideoCompressionPropertiesKey: [AVVideoAverageBitRateKey: 3_000_000,
                                      AVVideoMaxKeyFrameIntervalKey: 30]
])
input.expectsMediaDataInRealTime = false
let adaptor = AVAssetWriterInputPixelBufferAdaptor(assetWriterInput: input,
    sourcePixelBufferAttributes: [kCVPixelBufferPixelFormatTypeKey as String: kCVPixelFormatType_32BGRA,
                                  kCVPixelBufferWidthKey as String: width,
                                  kCVPixelBufferHeightKey as String: height])
writer.add(input)
guard writer.startWriting() else { fatalError("Cannot start encoder: \(String(describing: writer.error))") }
writer.startSession(atSourceTime: .zero)
var index: Int64 = 0
while let data = try reader.read(upToCount: width * height * 4), !data.isEmpty {
    precondition(data.count == width * height * 4, "Incomplete frame")
    while !input.isReadyForMoreMediaData {
        if writer.status == .failed { fatalError("Encoder failed: \(String(describing: writer.error))") }
        Thread.sleep(forTimeInterval: 0.002)
    }
    var pixelBuffer: CVPixelBuffer?
    guard let pool = adaptor.pixelBufferPool,
          CVPixelBufferPoolCreatePixelBuffer(nil, pool, &pixelBuffer) == kCVReturnSuccess,
          let buffer = pixelBuffer else { fatalError("Pixel buffer allocation failed") }
    CVPixelBufferLockBaseAddress(buffer, [])
    let destination = CVPixelBufferGetBaseAddress(buffer)!
    let stride = CVPixelBufferGetBytesPerRow(buffer)
    data.withUnsafeBytes { bytes in
        for row in 0..<height {
            memcpy(destination.advanced(by: row * stride), bytes.baseAddress!.advanced(by: row * width * 4), width * 4)
        }
    }
    CVPixelBufferUnlockBaseAddress(buffer, [])
    guard adaptor.append(buffer, withPresentationTime: CMTime(value: index, timescale: fps))
    else { fatalError("Cannot encode frame: \(String(describing: writer.error))") }
    index += 1
}
input.markAsFinished()
let completion = DispatchSemaphore(value: 0)
writer.finishWriting { completion.signal() }
completion.wait()
guard writer.status == .completed else { fatalError("Encoding failed: \(String(describing: writer.error))") }
print("Encoded \(index) frames (\(Double(index)/Double(fps)) seconds) to \(output.path)")

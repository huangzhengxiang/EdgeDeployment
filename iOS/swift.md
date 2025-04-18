## Swift
https://docs.swift.org/swift-book/documentation/the-swift-programming-language

iOS recommended programming language

### UI
The UI object can only be modified in the main actor task, if trying to modified elsewhere, a decorator is needed. Please specify `@MainActor`.
```swift
    Task { @MainActor in
        let prefillStartTime = CFAbsoluteTimeGetCurrent()
        llm?.forward(Int32(prefill_len), is_prefill: true, is_first_prefill: true)
        let prefillEndTime = CFAbsoluteTimeGetCurrent()
        let decodeStartTime = CFAbsoluteTimeGetCurrent()
        llm?.forward(Int32(decode_len), is_prefill: false, is_first_prefill: false)
        let decodeEndTime = CFAbsoluteTimeGetCurrent()
        llm?.reset()
        callback();
    }
```

### time measurement

Apple sysdiagnose timestamp vs swift `CFAbsoluteTime`.

sysdiagnose timestamp is `timeIntervalSince1970`, while `CFAbsoluteTime` is something like `timeIntervalSince2001`, differing by 978,307,200 seconds.

```swift
var swift2sysdiagnose = 978307200.0
startTime += swift2diagnose // convert to starting from 1970.
```
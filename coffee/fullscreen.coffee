goFullscreen = ->
    viewport.requestFullscreen = viewport.requestFullscreen or viewport.mozRequestFullscreen or viewport.mozRequestFullScreen or viewport.webkitRequestFullscreen
    viewport.requestFullscreen()

fullscreenChange = ->
    if document.webkitFullscreenElement is viewport or document.mozFullscreenElement is viewport or document.mozFullScreenElement is viewport
        lockPointer()

document.addEventListener 'fullscreenchange', fullscreenChange, false
document.addEventListener 'mozfullscreenchange', fullscreenChange, false
document.addEventListener 'webkitfullscreenchange', fullscreenChange, false

lockPointer = ->
    viewport.requestPointerLock = viewport.requestPointerLock or viewport.mozRequestPointerLock or viewport.webkitRequestPointerLock
    viewport.requestPointerLock()

pointerLockChange = ->
    if document.mozPointerLockElement is viewport or document.webkitPointerLockElement is viewport
        console.log "Pointer Lock was successful."
    else
        console.log "Pointer Lock was lost."

document.addEventListener 'pointerlockchange', pointerLockChange, false
document.addEventListener 'mozpointerlockchange', pointerLockChange, false
document.addEventListener 'webkitpointerlockchange', pointerLockChange, false

pointerLockError = (e) -> console.log "Error while locking pointer.", e

document.addEventListener 'pointerlockerror', pointerLockError, false
document.addEventListener 'mozpointerlockerror', pointerLockError, false
document.addEventListener 'webkitpointerlockerror', pointerLockError, false

$(viewport).click lockPointer

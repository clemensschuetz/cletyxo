#![no_std]
#![feature(asm)]

/// # Uasm - Unsafe Assembly
/// Exists to control the asm!() macro
macro_rules! uasm {
    ($($arg:tt)*) => { unsafe { asm!($($arg)*); } }
}


pub fn hlt() {
    uasm!("hlt");
}
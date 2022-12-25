num = 1

=begin
num.upto(48) {
  |i| puts <<~ISR
isr_#{i}:
	cli
	push #{i}
	jmp isr_basic
ISR

}
=end

num.upto(48) {
  |i| puts <<IDT
	dw isr_#{i}, 8, 0x8e00, 0x0000
IDT
}


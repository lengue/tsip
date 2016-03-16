# Introduction #

本来协议栈和应用层是同步处理的，但是今天发现一个问题，同步处理如果应用层立即回响应。因为协议栈的状态机还未完成，处理会发生错误。
改天还是把它改为异步处理好了


# Details #

Add your content here.  Format your content with:
  * Text in **bold** or _italic_
  * Headings, paragraphs, and lists
  * Automatic links to other wiki pages
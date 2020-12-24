using System;
using System.Collections.Generic;
using System.Text;

namespace cppcx.Core.Models
{
    public class CxGlobalVariable
    {
        public CxTypedName TypedName { get; set; }
        public string InitValue { get; set; } = null;
        public bool IsExtern { get; set; } = false;
    }
}

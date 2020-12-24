using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cppcx.Core.Models
{
    public class CxField
    {
        public CxTypedName TypedName { get; set; }
        public string InitValue { get; set; } = null;
        public CxVisibility Visibility { get; set; } = CxVisibility.Public;
        public bool IsStatic { get; set; } = false;
    }
}
